#include <glad/glad.h> // slave of testosterone
#include <glad/glad_glx.h> // slave of testosterone
#include <GL/freeglut.h> // almost? fix for slave of testosterone
#include <X11/Xlib.h>
#include <X11/extensions/Xcomposite.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <vector>

#include <obs-module.h>
#include <graphics/vec4.h>
#include <util/platform.h>

#include "ca2plugin-main.hpp"
#include "ca2plugin-helper.hpp"

#define xdisp (ca2plugin::disp())
#define WIN_STRING_DIV "\r\n"


bool ca2pluginMain::init()
{
	if (!xdisp) {
		blog(LOG_ERROR, "failed opening display");
		return false;
	}

	int eventBase, errorBase;
	if (!XCompositeQueryExtension(xdisp, &eventBase, &errorBase)) {
		blog(LOG_ERROR, "Xcomposite extension not supported");
		return false;
	}

	int major = 0, minor = 2;
	XCompositeQueryVersion(xdisp, &major, &minor);

	if (major == 0 && minor < 2) {
		blog(LOG_ERROR, "Xcomposite extension is too old: %d.%d < 0.2",
				major, minor);
		return false;
	}

	return true;
}

void ca2pluginMain::deinit()
{
	ca2plugin::cleanupDisplay();
}

obs_properties_t *ca2pluginMain::properties()
{
	obs_properties_t *props = obs_properties_create();

	obs_property_t *wins = obs_properties_add_list(props, "capture_window",
			obs_module_text("Window"),
			OBS_COMBO_TYPE_LIST, OBS_COMBO_FORMAT_STRING);

	for (Window win: ca2plugin::getTopLevelWindows()) {
		std::string wname = ca2plugin::getWindowName(win);
		std::string cls = ca2plugin::getWindowClass(win);
		std::string winid = std::to_string((long long)win);
		std::string desc =
			(winid + WIN_STRING_DIV + wname +
			 WIN_STRING_DIV + cls);

		obs_property_list_add_string(wins, wname.c_str(),
				desc.c_str());
	}

	obs_properties_add_int(props, "cut_top", obs_module_text("CropTop"),
			0, 4096, 1);
	obs_properties_add_int(props, "cut_left", obs_module_text("CropLeft"),
			0, 4096, 1);
	obs_properties_add_int(props, "cut_right", obs_module_text("CropRight"),
			0, 4096, 1);
	obs_properties_add_int(props, "cut_bot", obs_module_text("CropBottom"),
			0, 4096, 1);

	obs_properties_add_bool(props, "swap_redblue",
			obs_module_text("SwapRedBlue"));
	obs_properties_add_bool(props, "lock_x", obs_module_text("LockX"));

	obs_properties_add_bool(props, "show_cursor",
			obs_module_text("CaptureCursor"));

	obs_properties_add_bool(props, "include_border",
			obs_module_text("IncludeXBorder"));

	obs_properties_add_bool(props, "exclude_alpha",
			obs_module_text("ExcludeAlpha"));

	return props;
}

void ca2pluginMain::defaults(obs_data_t *settings)
{
	obs_data_set_default_string(settings, "capture_window", "");
	obs_data_set_default_int(settings, "cut_top", 0);
	obs_data_set_default_int(settings, "cut_left", 0);
	obs_data_set_default_int(settings, "cut_right", 0);
	obs_data_set_default_int(settings, "cut_bot", 0);
	obs_data_set_default_bool(settings, "swap_redblue", false);
	obs_data_set_default_bool(settings, "lock_x", false);
	obs_data_set_default_bool(settings, "show_cursor", true);
	obs_data_set_default_bool(settings, "include_border", false);
	obs_data_set_default_bool(settings, "exclude_alpha", false);
}

#define FIND_WINDOW_INTERVAL 2.0

struct ca2pluginMain_private
{
	ca2pluginMain_private()
		:win(0)
		,cut_top(0), cur_cut_top(0)
		,cut_left(0), cur_cut_left(0)
		,cut_right(0), cur_cut_right(0)
		,cut_bot(0), cur_cut_bot(0)
		,inverted(false)
		,width(0),height(0)
		,pixmap(0)
		,glxpixmap(0)
		,tex(0)
		,gltex(0)
	{
		pthread_mutexattr_init(&lockattr);
		pthread_mutexattr_settype(&lockattr, PTHREAD_MUTEX_RECURSIVE);

		pthread_mutex_init(&lock, &lockattr);
	}

	~ca2pluginMain_private()
	{
		
		close_map();

		pthread_mutex_destroy(&lock);

		pthread_mutexattr_destroy(&lockattr);

	}

	obs_source_t *source;

	uint32_t * m_p;

	int m_iFileSize;

	int m_iFile;
	
	void close_map();

	std::string windowName;

	Window win = 0;

	Window winMap = 0;

	double window_check_time = 0.0;

	uint32_t width;
	uint32_t height;

	gs_texture_t *tex;

	pthread_mutex_t lock;
	pthread_mutexattr_t lockattr;

};


ca2pluginMain::ca2pluginMain(obs_data_t *settings, obs_source_t *source)
{
	
	p = new ca2pluginMain_private;
	
	p->source = source;

	updateSettings(settings);

}


static void xcc_cleanup(ca2pluginMain_private *p);

ca2pluginMain::~ca2pluginMain()
{
	ObsGsContextHolder obsctx;

	if (p->tex) {
		gs_texture_destroy(p->tex);
		p->tex = 0;
	}

	xcc_cleanup(p);

	delete p;

}

static Window getWindowFromString(std::string wstr)
{
	XErrorLock xlock;

	if (wstr == "") {
		return ca2plugin::getTopLevelWindows().front();
	}

	if (wstr.substr(0, 4) == "root") {
		int i = std::stoi("0" + wstr.substr(4));
		return RootWindow(xdisp, i);
	}

	size_t firstMark = wstr.find(WIN_STRING_DIV);
	size_t markSize = strlen(WIN_STRING_DIV);

	if (firstMark == std::string::npos)
		return (Window)std::stol(wstr);

	Window wid = 0;

	wstr = wstr.substr(firstMark + markSize);

	size_t lastMark = wstr.rfind(WIN_STRING_DIV);
	std::string wname = wstr.substr(0, lastMark);
	std::string wcls = wstr.substr(lastMark + markSize);

	Window matchedNameWin = wid;
	for (Window cwin: ca2plugin::getTopLevelWindows()) {
		std::string cwinname = ca2plugin::getWindowName(cwin);
		std::string ccls = ca2plugin::getWindowClass(cwin);

		if (cwin == wid && wname == cwinname && wcls == ccls)
			return wid;

		if (wname == cwinname ||
		    (!matchedNameWin && !wcls.empty() && wcls == ccls))
			matchedNameWin = cwin;
	}

	return matchedNameWin;
}

static void xcc_cleanup(ca2pluginMain_private *p)
{
}
  /* reverse:  reverse string s in place */
 void reverse(char *s)
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}  
 void itoa64(int64_t n, char *s)
 {

     int64_t i;
	  int64_t sign = n;

     if (sign  < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}  

void ca2pluginMain_private::close_map()
{
   
	if(m_p != (uint32_t *)MAP_FAILED)
   {

      ::munmap(m_p, m_iFileSize);

      m_p = (uint32_t *) MAP_FAILED;

		m_iFileSize = -1;

   }

   if(m_iFile != -1)
   {

      ::close(m_iFile);

      m_iFile = -1;

   }
	
}


void ca2pluginMain::updateSettings(obs_data_t *settings)
{

	PLock lock(&p->lock);

	XErrorLock xlock;
	
	ObsGsContextHolder obsctx;

	blog(LOG_DEBUG, "Settings updating");

	Window prevWin = p->win;

	xcc_cleanup(p);

	if (settings)
	{
	
		const char * windowName = obs_data_get_string(settings, "capture_window");

		p->windowName = windowName;

		p->win = getWindowFromString(windowName);

	} 
	else
	{

		p->win = prevWin;

	}

	xlock.resetError();

	if (!p->win)
	{

		p->win = 0;

		p->width = 0;

		p->height = 0;

		return;

	}

	p->width = 0;

	p->height = 0;

	p->close_map();

}


void ca2pluginMain::tick(float seconds)
{

	if (!obs_source_showing(p->source))
		return;

	PLock lock(&p->lock, true);

	if (!lock.isLocked())
		return;


	ca2plugin::processEvents();

	if (p->win && ca2plugin::windowWasReconfigured(p->win))
	{

		p->window_check_time = FIND_WINDOW_INTERVAL;

		p->win = 0;

	}

	XDisplayLock xlock;
	
	XWindowAttributes attr;

	if (!p->win || !XGetWindowAttributes(xdisp, p->win, &attr))
	{

		p->window_check_time += (double)seconds;

		if (p->window_check_time < FIND_WINDOW_INTERVAL)
			return;

		Window newWin = getWindowFromString(p->windowName);

		p->window_check_time = 0.0;

		if (newWin && XGetWindowAttributes(xdisp, newWin, &attr))
		{
			
			p->win = newWin;

			updateSettings(0);

		} 
		else
		{
			
			return;

		}
	}

	if(p->m_p == (uint32_t *) MAP_FAILED || p->win != p->winMap)
	{

		if(p->m_p != (uint32_t *) MAP_FAILED)
		{
		
			p->close_map();

		}

		if(p->win == 0)
		{

			p->winMap = 0;

			return;

		}

		char sz[1024];
		char sz2[1024];
		char szCmd[1024];
		char szId[64];

		strcpy(sz, "/var/tmp/ca2");
		strcat(sz, getenv("HOME"));
		strcat(sz, "/ca2screen-");

		itoa64(p->win,szId);
		strcat(sz, szId);

		p->m_iFileSize = 8192 * 4096 * 4;

   	p->m_iFile = ::open(sz, O_RDONLY);

	   if(p->m_iFile == -1)
   	{

      	p->close_map();

			return;

	   }

		strcpy(sz2, sz);
		strcat(sz2, ".fileidcreated");

		strcpy(szCmd, "touch ");
		strcat(szCmd, sz2);
		system(szCmd);

   	p->m_p = (uint32_t *)mmap(NULL,p->m_iFileSize, PROT_READ,MAP_SHARED,p->m_iFile,0);

		if(p->m_p == (uint32_t*) MAP_FAILED)
		{

			p->close_map();

			return;

		}

		int64_t * pdata = (int64_t *)p->m_p;
		
		strcpy(sz2, sz);
		strcat(sz2, ".mapsucceeded");
		strcpy(szCmd, "touch ");
		strcat(szCmd, sz2);
		system(szCmd);

		strcpy(sz2, sz);
		strcat(sz2, ".w");
		p->width = *pdata++; 
		itoa64(p->width,szId);
		strcat(sz2, szId);
		strcpy(szCmd, "touch ");
		strcat(szCmd, sz2);
		system(szCmd);

		strcpy(sz2, sz);
		strcat(sz2, ".h");
		p->height = *pdata++;
		itoa64(p->height,szId);
		strcat(sz2, szId);
		strcpy(szCmd, "touch ");
		strcat(szCmd, sz2);
		system(szCmd);

		if(p->width == 0 || p->width > 4096)
		{

			return;

		}

		if(p->height == 0 || p->height > 4096)
		{

			return;

		}

		strcpy(sz2, sz);
		strcat(sz2, ".s");
		itoa64(*pdata++,szId);

		strcat(sz2, szId);

		strcpy(szCmd, "touch ");
		strcat(szCmd, sz2);
		system(szCmd);

		strcpy(sz2, sz);
		strcat(sz2, ".first64bits");
		itoa64(*pdata++,szId);

		strcat(sz2, szId);

		strcpy(szCmd, "touch ");
		strcat(szCmd, sz2);
		system(szCmd);

		ObsGsContextHolder obsctx;

		gs_color_format cf = GS_RGBA;

		if (p->tex)
		{

			gs_texture_destroy(p->tex);

		}

		uint8_t *texData = new uint8_t[width() * height() * 4];

		memset(texData, 0, width() * height() * 4);

		const uint8_t* texDataArr[] = { texData, 0 };

		p->tex = gs_texture_create(width(), height(), cf, 1, texDataArr, 0);

		delete[] texData;

		p->winMap = p->win;

	}
	
	if (!p->tex)
	{

		return;

	}

	obs_enter_graphics();

	uint32_t * pcolorref = p->m_p  == (uint32_t*)MAP_FAILED? NULL: p->m_p;

	if(pcolorref)
	{

		try
		{
		
			int64_t * pdata = (int64_t *)pcolorref;

			int64_t cx = *pdata++;

			int64_t cy = *pdata++;

			int64_t scan = *pdata++;

			GLuint t = *(GLuint*)gs_texture_get_obj(p->tex);
		
			glBindTexture(GL_TEXTURE_2D, t);

			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width(), height(),  GL_BGRA, GL_UNSIGNED_BYTE, p->m_p);

		}
		catch(...)
		{

		}

	}

	obs_leave_graphics();

}


void ca2pluginMain::render(gs_effect_t *effect)
{

	if (!p->win)
	{

		return;

	}

	PLock lock(&p->lock, true);

	effect = obs_get_base_effect(OBS_EFFECT_PREMULTIPLIED_ALPHA);

	if (!lock.isLocked() || !p->tex)
	{

		return;

	}

	gs_eparam_t *image = gs_effect_get_param_by_name(effect, "image");

	gs_effect_set_texture(image, p->tex);

	while (gs_effect_loop(effect, "Draw"))
	{

		gs_draw_sprite(p->tex, 0, 0, 0);

	}

}


uint32_t ca2pluginMain::width()
{
	if (!p->win)
		return 0;

	return p->width;
}

uint32_t ca2pluginMain::height()
{
	if (!p->win)
		return 0;

	return p->height;
}



obs_source_t *get_transition(ca2pluginMain *ss)
{
   obs_source_t *tr;

	PLock lock(&ss->p->lock);
   tr = ss->p->source;
   obs_source_addref(tr);
   
   return tr;
}


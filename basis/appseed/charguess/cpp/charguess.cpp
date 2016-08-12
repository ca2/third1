/*
	libcharguess	-	Guess the encoding/charset of a string
    Copyright (C) 2003  Stephane Corbe <noubi@users.sourceforge.net>
	Based on Mozilla sources

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

//#include "framework.h"




extern "C" charguess_det CharGuessInit()
{

   nsUniversalDetector* det = new nsUniversalDetector;

	if (det != NULL)
      return (charguess_det) det;
	else
		return NULL;

}

const char* GuessChardet(charguess_det p,const string & str)
{
   
   nsUniversalDetector* det = (nsUniversalDetector*) p;

	det->Reset();
	det->HandleData(str, str.length());
	det->DataEnd();
	
   return det->GetCharset();

}

extern "C" const char* GuessChardet(charguess_det p,const char *str)
{
   
   return GuessChardet(p,(const string &)str);

}

extern "C" void CharGuessDestroy(charguess_det p)
{

   try
   {

      nsUniversalDetector* det = (nsUniversalDetector*)p;

      if(det != NULL)
         delete det;

   }
   catch(...)
   {

   }

}





#ifdef _DEBUG

#define TEST_LATIN1 "Aucun fichier ou répertoire de ce type"
#define TEST_UTF8   "æ–™ç†ã«åˆã‚ã›ãŸç¾å‘³ã—ã„ãƒ¯ã‚¤ãƒ³ã®é¸ã³æ–¹ã€‚"
#define TEST_SJIS   "—¿—‚É‡‚í‚¹‚½”ü–¡‚µ‚¢ƒƒCƒ“‚Ì‘I‚Ñ•ûB"
#define TEST_JIS    "$BNAM}$K9g$o$;$?H~L#$7$$%o%$%s$NA*$SJ}!#(B"

void _debug_charguess()
{

   //uint32_t uiCodePage = charguess("\"Carlos é brasileiro\" está escrito em um código de página latino?")();

}

#endif





uint32_t charguess::get_code_page(const string & str)
{
   if(str.is_empty())
   {
      return 0;
   }
   else if(!str.CompareNoCase("Shift_JIS"))
   {
      return 932;
   }
   else if(!str.CompareNoCase("Big5"))
   {
      return 950;
   }
   else if(!str.CompareNoCase("gb18030"))
   {
      return 936;
   }
   else if(!str.CompareNoCase("windows-1252"))
   {
      return 1252;
   }
   else if(!str.CompareNoCase("UTF-8"))
   {
      return CP_UTF8;
   }
   else
   {
      return 0;
   }
}

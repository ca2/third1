
class png_error_exception
{

public:

};

extern "C"
void throw_error_exception()
{

   throw png_error_exception();

}
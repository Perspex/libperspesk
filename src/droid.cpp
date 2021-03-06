#include "common.h"
namespace libperspesk
{
#ifdef __ANDROID__
	extern  JNIEnv *Jni = 0;

	static  jobject jBitmapConfig;

	void SwViewport::DrawToWindow()
	{
		VERBOSE("Releasing native window");
		Surface.reset();
		ANativeWindow_unlockAndPost(NativeWindow);
		ANativeWindow_release(NativeWindow);
	}

	extern void InitSw()
	{
	}

	void SwViewport::Setup()
	{
	}

	void SwViewport::Rezise(int width, int height)
	{
	}

	extern void GetPlatformWindowDimensions(void* nativeHandle, int* width, int* height)
	{
		ANativeWindow* w = ANativeWindow_fromSurface(Jni, (jobject)nativeHandle);
		VERBOSE("Got native window for measurements %p", w);
		*width = ANativeWindow_getWidth(w);
		*height = ANativeWindow_getHeight(w);
		ANativeWindow_release(w);
	}
	void SwViewport::PrepareToDraw() {

		NativeWindow = ANativeWindow_fromSurface(Jni, (jobject)Window);

		VERBOSE("Got native window %p", NativeWindow);
		int width = ANativeWindow_getWidth(NativeWindow);
		int height = ANativeWindow_getHeight(NativeWindow);
		int format = ANativeWindow_getFormat(NativeWindow);
		VERBOSE("Size: %ix%i, Format: %i", width, height, format);
		ANativeWindow_Buffer buffer;
		ARect rc;
		rc.left = 0;
		rc.top =0;
		rc.right = width;
		rc.bottom = height;
		buffer.bits = nullptr;
		ANativeWindow_lock(NativeWindow, &buffer, &rc);
		VERBOSE("Locked data: %p", buffer.bits);
		SkColorType color = SKIA_COLOR_FORMAT;
		int stride  = buffer.stride;

		if(format == WINDOW_FORMAT_RGB_565)
		{
			color = kRGB_565_SkColorType;
			stride*=2;
		}
		else
			stride *= 4;
		VERBOSE("Using color format: %i, stride: %i", color, stride);



		Surface.reset(SkSurface::NewRasterDirect(SkImageInfo::Make(width, height, color, kOpaque_SkAlphaType),
		    buffer.bits, stride));
		VERBOSE("Surface: %p", Surface.get());
	}
	SwViewport::~SwViewport()
	{
	}

	SwViewport::SwViewport()
	{
	}



#endif
}

#ifdef __ANDROID__
extern "C" void PerspexJniInit(JNIEnv *env)
{
	Jni = env;
}
#endif
/**
 * The MIT License (MIT)
 * <p/>
 * Copyright (c) 2015 Bertrand Martel
 * <p/>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * <p/>
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * <p/>
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
/*
	Parse a bluetooth snoop file for HCI frames into json format 

	@author Bertrand Martel
	@version 0.1

*/
#ifdef __ANDROID__

#include <android/log.h> 
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

/*wrapper for HCI frame decoder from a snoop file*/
JNIEXPORT jstring JNICALL Java_fr_bmartel_android_hcidecoder_MainActivity_decodeHci(JNIEnv* env, jclass cls,jstring snoop_file)
{
	const char *snoop_file_str = env->GetStringUTFChars(snoop_file, JNI_FALSE);

	__android_log_print(ANDROID_LOG_VERBOSE,"hcidecoder","decoding btsnoop file : %s\n",snoop_file_str);

	env->ReleaseStringUTFChars(snoop_file, snoop_file_str);

	return env->NewStringUTF("");
}

#ifdef __cplusplus
}
#endif

#endif /* __ANDROID__ */
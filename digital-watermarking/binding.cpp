#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject* transformImageWithText(PyObject *self, PyObject *args, PyObject *kwargs) {
    v8::String::Utf8Value str0(info[0]->ToString());
    const char* filename= ToCString(str0);
    v8::String::Utf8Value str1(info[1]->ToString());
    const char* watermarkText= ToCString(str1);
    double fontSize = info[2]->NumberValue();
    v8::String::Utf8Value str3(info[3]->ToString());
    const char* outfilename= ToCString(str3);
    cv::Mat srcImg = cv::imread(filename);
    if (srcImg.empty()){Nan::ThrowTypeError("read image failed");}
    cv::Mat comImg = transFormMatWithText(srcImg, watermarkText,fontSize);
    bool res = cv::imwrite(outfilename,comImg);
    info.GetReturnValue().Set(res);
}

static PyObject* getTextFormImage(PyObject *self, PyObject *args, PyObject *kwargs) {
    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    v8::String::Utf8Value str0(info[0]->ToString());
    const char* filename= ToCString(str0);
    v8::String::Utf8Value str1(info[1]->ToString());
    const char* backfilename= ToCString(str1);

    cv::Mat comImg = cv::imread(filename);
    cv::Mat backImage = getTextFormMat(comImg);
    bool res = cv::imwrite(backfilename,backImage);
    info.GetReturnValue().Set(res);
}

static PyMethodDef DigitalWatermarkingMethods[] = {
    {"transform_image_with_text",  transformImageWithText, METH_VARARGS | METH_KEYWORDS,
     "Embeds a text into image"},
    {"get_text_from_image",  getTextFormImage, METH_VARARGS | METH_KEYWORDS,
     "Extracts text from image"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef digitalwatermarkingmodule = {
    PyModuleDef_HEAD_INIT,
    "digitalwatermarking",   /* name of module */
    NULL,                    /* module documentation, may be NULL */
    -1,                      /* size of per-interpreter state of the module,
                             or -1 if the module keeps state in global variables. */
    DigitalWatermarkingMethods
};

PyMODINIT_FUNC PyInit_digitalwatermarking(void)
{
    return PyModule_Create(&digitalwatermarkingmodule);
}
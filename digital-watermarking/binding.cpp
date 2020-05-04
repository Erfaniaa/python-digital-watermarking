#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "watermarker.h"

static PyObject* transformImageWithText(PyObject *self, PyObject *args, PyObject *kwargs) {
    char *filename;
    char *watermarkText;
    double fontSize;
    char* outfilename;

    static char *kwlist[] = {"filename", "text", "font_size", "output", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ssds", kwlist, &filename, &watermarkText, &fontSize, &outfilename))
        return NULL;

    cv::Mat srcImg = cv::imread(filename);
    cv::Mat comImg = transFormMatWithText(srcImg, watermarkText,fontSize);
    bool res = cv::imwrite(outfilename,comImg);

    if (res)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject* getTextFormImage(PyObject *self, PyObject *args, PyObject *kwargs) {
    char *filename;
    char *backfilename;
    double fontSize;

    static char *kwlist[] = {"filename", "backfilename", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss", kwlist, &filename, &backfilename))
        return NULL;

    cv::Mat comImg = cv::imread(filename);
    cv::Mat backImage = getTextFormMat(comImg);
    bool res = cv::imwrite(backfilename,backImage);

    if (res)
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyMethodDef DigitalWatermarkingMethods[] = {
    {"transform_image_with_text",  (PyCFunction)transformImageWithText, METH_VARARGS | METH_KEYWORDS,
     "Embeds a text into image"},
    {"get_text_from_image",  (PyCFunction)getTextFormImage, METH_VARARGS | METH_KEYWORDS,
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

PyMODINIT_FUNC PyInit_digitalwatermarking(void) {
    return PyModule_Create(&digitalwatermarkingmodule);
}
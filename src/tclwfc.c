#include <stdint.h>

#include <tcl.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#define WFC_IMPLEMENTATION
#define WFC_USE_STB
#include "wfc.h"


static struct wfc *wfc = NULL;
static struct wfc_image *inputImage = NULL;

static int Tclwfc_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    if (objc < 9)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "wfc");
        return TCL_ERROR;
    }

	int result;

    int width;
	result = Tcl_GetIntFromObj(interp, objv[1], &width);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

    int height;
	result = Tcl_GetIntFromObj(interp, objv[2], &height);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

	char *fileName;
	fileName = Tcl_GetString(objv[3]);

    int tileWidth;
	result = Tcl_GetIntFromObj(interp, objv[4], &tileWidth);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

    int tileHeight;
	result = Tcl_GetIntFromObj(interp, objv[5], &tileHeight);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

    int expandInput;
	result = Tcl_GetIntFromObj(interp, objv[6], &expandInput);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

    int addHoriz;
	result = Tcl_GetIntFromObj(interp, objv[7], &addHoriz);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

    int addVert;
	result = Tcl_GetIntFromObj(interp, objv[8], &addVert);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

    int add90Rotations;
	result = Tcl_GetIntFromObj(interp, objv[9], &add90Rotations);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

	inputImage = wfc_img_load(fileName);

    wfc = wfc_overlapping(
		 width,
		 height,
		 inputImage,
		 tileWidth,
		 tileHeight,
		 expandInput,
		 addHoriz,
		 addVert,
		 add90Rotations
	 );

    return TCL_OK;
}

static int Tclwfc_Gen(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
	wfc_init(wfc);

	int maxIters = -1;

	if (objc == 2)
	{
		int result;
		result = Tcl_GetIntFromObj(interp, objv[1], &maxIters);
		if (result != TCL_OK)
		{
			return TCL_ERROR;
		}
	}

    wfc_run(wfc, maxIters);

	Tcl_Obj *pixels = Tcl_NewListObj(0, NULL);

	struct wfc_image *tmpImg = wfc_output_image(wfc);

	for (int height = 0; height < tmpImg->height; height++)
	{
		for (int width = 0; width < tmpImg->width; width++)
		{
			//uint8_t color = tmpImg->data[(tmpImg->width * height + width) * tmpImg->component_cnt];
			uint32_t color = *(uint32_t*)&tmpImg->data[(tmpImg->width * height + width) * tmpImg->component_cnt];
			Tcl_ListObjAppendElement(interp, pixels, Tcl_NewIntObj(color));
		}
	}

	 wfc_img_destroy(tmpImg);

	Tcl_SetObjResult(interp, pixels);

	return TCL_OK;
}

#if 0
         wfc_destroy(wfc);
         wfc_img_destroy(output_image);
#endif

int DLLEXPORT Tclwfc_Init(Tcl_Interp *interp)
{
    if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL)
	{
        return TCL_ERROR;
    }

    if (Tcl_PkgProvide(interp, "tclwfc", "1.0") == TCL_ERROR)
	{
        return TCL_ERROR;
    }

    Tcl_CreateObjCommand(interp, "tclwfc::wfc", Tclwfc_Cmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, "tclwfc::generate", Tclwfc_Gen, NULL, NULL);

    return TCL_OK;
}

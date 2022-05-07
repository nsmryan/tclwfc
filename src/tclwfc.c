#include <stdint.h>

#include <tcl.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#define WFC_IMPLEMENTATION
#define WFC_USE_STB
#include "wfc.h"


typedef struct Tclwfc_State
{
    struct wfc *wfc;
    struct wfc_image *image;
} Tclwfc_State;


static int Tclwfc_Gen(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);

static void Tclwfc_StateDelete(ClientData cdata)
{
    Tclwfc_State *state = (Tclwfc_State*)cdata;
    
     wfc_destroy(state->wfc);
     wfc_img_destroy(state->image);
}

static int Tclwfc_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    if (objc < 10)
    {
        Tcl_WrongNumArgs(interp, 1, objv, "wfc cmd width height fileName tileWidth tileHeight expandInput addHoriz addVert add90DegreeRotations");
        return TCL_ERROR;
    }

	int result;

    int objIndex = 1;

	char *cmdName;
	cmdName = Tcl_GetString(objv[objIndex++]);

    int width;
	result = Tcl_GetIntFromObj(interp, objv[objIndex++], &width);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

    int height;
	result = Tcl_GetIntFromObj(interp, objv[objIndex++], &height);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

	char *fileName;
	fileName = Tcl_GetString(objv[objIndex++]);

    int tileWidth;
	result = Tcl_GetIntFromObj(interp, objv[objIndex++], &tileWidth);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

    int tileHeight;
	result = Tcl_GetIntFromObj(interp, objv[objIndex++], &tileHeight);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

    int expandInput;
	result = Tcl_GetIntFromObj(interp, objv[objIndex++], &expandInput);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

    int addHoriz;
	result = Tcl_GetIntFromObj(interp, objv[objIndex++], &addHoriz);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

    int addVert;
	result = Tcl_GetIntFromObj(interp, objv[objIndex++], &addVert);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

    int add90Rotations;
	result = Tcl_GetIntFromObj(interp, objv[objIndex++], &add90Rotations);
	if (result != TCL_OK)
	{
        return TCL_ERROR;
	}

	struct wfc_image *image = wfc_img_load(fileName);

    struct wfc *wfc = wfc_overlapping(
		 width,
		 height,
		 image,
		 tileWidth,
		 tileHeight,
		 expandInput,
		 addHoriz,
		 addVert,
		 add90Rotations
	 );

    Tclwfc_State *state = malloc(sizeof(Tclwfc_State));
    if (state == NULL)
    {
        return TCL_ERROR;
    }

    state->wfc = wfc;
    state->image = image;

    Tcl_CreateObjCommand(interp, cmdName, Tclwfc_Gen, state, Tclwfc_StateDelete);

    return TCL_OK;
}

static int Tclwfc_Gen(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    Tclwfc_State *state = (Tclwfc_State*)cdata;

	wfc_init(state->wfc);

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

    wfc_run(state->wfc, maxIters);

	Tcl_Obj *pixels = Tcl_NewListObj(0, NULL);

	struct wfc_image *tmpImg = wfc_output_image(state->wfc);

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

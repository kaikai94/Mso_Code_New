




/*==========================================================================;
 *
 *  Copyright (C) Microsoft Corporation.  All Rights Reserved.
 *
 *  File:   dx7todx8.h
 *  Content:    DX7 to DX8 Direct3D aliases to aid porting DX7 apps to DX8
 *
 ***************************************************************************/

#ifndef _DX7TODX8_H_

///////////////////////////////////////////////////////////////////////////////
//
// d3d8types.h
//
///////////////////////////////////////////////////////////////////////////////

#define D3DTRANSFORMSTATE_WORLD                 D3DTS_WORLD
#define D3DTRANSFORMSTATE_VIEW                  D3DTS_VIEW
#define D3DTRANSFORMSTATE_PROJECTION            D3DTS_PROJECTION
#define D3DTRANSFORMSTATE_WORLD1                D3DTS_WORLD1
#define D3DTRANSFORMSTATE_WORLD2                D3DTS_WORLD2
#define D3DTRANSFORMSTATE_WORLD3                D3DTS_WORLD3
#define D3DTRANSFORMSTATE_TEXTURE0              D3DTS_TEXTURE0
#define D3DTRANSFORMSTATE_TEXTURE1              D3DTS_TEXTURE1
#define D3DTRANSFORMSTATE_TEXTURE2              D3DTS_TEXTURE2
#define D3DTRANSFORMSTATE_TEXTURE3              D3DTS_TEXTURE3
#define D3DTRANSFORMSTATE_TEXTURE4              D3DTS_TEXTURE4
#define D3DTRANSFORMSTATE_TEXTURE5              D3DTS_TEXTURE5
#define D3DTRANSFORMSTATE_TEXTURE6              D3DTS_TEXTURE6

#define D3DTRANSFORMSTATE_TEXTURE7              D3DTS_TEXTURE7

#define D3DTRANSFORMSTATE_FORCE_DWORD           D3DTS_FORCE_DWORD

#define D3DRENDERSTATE_ZENABLE                  D3DRS_ZENABLE
#define D3DRENDERSTATE_FILLMODE                 D3DRS_FILLMODE
#define D3DRENDERSTATE_SHADEMODE                D3DRS_SHADEMODE
#define D3DRENDERSTATE_LINEPATTERN              D3DRS_LINEPATTERN
#define D3DRENDERSTATE_ZWRITEENABLE             D3DRS_ZWRITEENABLE
#define D3DRENDERSTATE_ALPHATESTENABLE          D3DRS_ALPHATESTENABLE
#define D3DRENDERSTATE_LASTPIXEL                D3DRS_LASTPIXEL

#define D3DRENDERSTATE_SRCBLEND                 D3DRS_SRCBLEND

#define D3DRENDERSTATE_DESTBLEND                D3DRS_DESTBLEND
#define D3DRENDERSTATE_CULLMODE                 D3DRS_CULLMODE
#define D3DRENDERSTATE_ZFUNC                    D3DRS_ZFUNC
#define D3DRENDERSTATE_ALPHAREF                 D3DRS_ALPHAREF
#define D3DRENDERSTATE_ALPHAFUNC                D3DRS_ALPHAFUNC
#define D3DRENDERSTATE_DITHERENABLE             D3DRS_DITHERENABLE
#define D3DRENDERSTATE_ALPHABLENDENABLE         D3DRS_ALPHABLENDENABLE
#define D3DRENDERSTATE_FOGENABLE                D3DRS_FOGENABLE
#define D3DRENDERSTATE_SPECULARENABLE           D3DRS_SPECULARENABLE
#define D3DRENDERSTATE_ZVISIBLE                 D3DRS_ZVISIBLE
#define D3DRENDERSTATE_FOGCOLOR                 D3DRS_FOGCOLOR
#define D3DRENDERSTATE_FOGTABLEMODE             D3DRS_FOGTABLEMODE

#define D3DRENDERSTATE_FOGSTART                 D3DRS_FOGSTART
#define D3DRENDERSTATE_FOGEND                   D3DRS_FOGEND
#define D3DRENDERSTATE_FOGDENSITY               D3DRS_FOGDENSITY
#define D3DRENDERSTATE_EDGEANTIALIAS            D3DRS_EDGEANTIALIAS
#define D3DRENDERSTATE_ZBIAS                    D3DRS_ZBIAS
#define D3DRENDERSTATE_RANGEFOGENABLE           D3DRS_RANGEFOGENABLE
#define D3DRENDERSTATE_STENCILENABLE            D3DRS_STENCILENABLE
#define D3DRENDERSTATE_STENCILFAIL              D3DRS_STENCILFAIL
#define D3DRENDERSTATE_STENCILZFAIL             D3DRS_STENCILZFAIL
#define D3DRENDERSTATE_STENCILPASS              D3DRS_STENCILPASS
#define D3DRENDERSTATE_STENCILFUNC              D3DRS_STENCILFUNC
#define D3DRENDERSTATE_STENCILREF               D3DRS_STENCILREF
#define D3DRENDERSTATE_STENCILMASK              D3DRS_STENCILMASK
#define D3DRENDERSTATE_STENCILWRITEMASK         D3DRS_STENCILWRITEMASK
#define D3DRENDERSTATE_TEXTUREFACTOR            D3DRS_TEXTUREFACTOR
#define D3DRENDERSTATE_WRAP0                    D3DRS_WRAP0
#define D3DRENDERSTATE_WRAP1                    D3DRS_WRAP1
#define D3DRENDERSTATE_WRAP2                    D3DRS_WRAP2
#define D3DRENDERSTATE_WRAP3                    D3DRS_WRAP3
#define D3DRENDERSTATE_WRAP4                    D3DRS_WRAP4
#define D3DRENDERSTATE_WRAP5                    D3DRS_WRAP5
#define D3DRENDERSTATE_WRAP6                    D3DRS_WRAP6
#define D3DRENDERSTATE_WRAP7                    D3DRS_WRAP7
#define D3DRENDERSTATE_CLIPPING                 D3DRS_CLIPPING

#define D3DRENDERSTATE_LIGHTING                 D3DRS_LIGHTING
#define D3DRENDERSTATE_EXTENTS                  D3DRS_EXTENTS
#define D3DRENDERSTATE_AMBIENT                  D3DRS_AMBIENT
#define D3DRENDERSTATE_FOGVERTEXMODE            D3DRS_FOGVERTEXMODE
#define D3DRENDERSTATE_COLORVERTEX              D3DRS_COLORVERTEX

#define D3DRENDERSTATE_LOCALVIEWER              D3DRS_LOCALVIEWER
#define D3DRENDERSTATE_NORMALIZENORMALS         D3DRS_NORMALIZENORMALS
#define D3DRENDERSTATE_DIFFUSEMATERIALSOURCE    D3DRS_DIFFUSEMATERIALSOURCE
#define D3DRENDERSTATE_SPECULARMATERIALSOURCE   D3DRS_SPECULARMATERIALSOURCE
#define D3DRENDERSTATE_AMBIENTMATERIALSOURCE    D3DRS_AMBIENTMATERIALSOURCE
#define D3DRENDERSTATE_EMISSIVEMATERIALSOURCE   D3DRS_EMISSIVEMATERIALSOURCE
#define D3DRENDERSTATE_VERTEXBLEND              D3DRS_VERTEXBLEND

#define D3DRENDERSTATE_CLIPPLANEENABLE          D3DRS_CLIPPLANEENABLE


#define RGBA_MAKE                               D3DCOLOR_RGBA
#define RGB_MAKE                                D3DCOLOR_XRGB
#define D3DRGBA                                 D3DCOLOR_COLORVALUE
#define D3DRGB(_r,_g,_b)                        D3DCOLOR_COLORVALUE(_r,_g,_b,1.f)

#define _DX7TODX8_H_
#endif //_DX7TODX8_H_


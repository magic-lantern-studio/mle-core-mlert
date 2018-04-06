/** @defgroup MleFoundation Magic Lantern Runtime Engine Foundation Library API */

/**
 * @file MleStageFuncs.h
 * @ingroup MleFoundation
 *
 * @author Mark S. Millard
 * @created Sep 20, 2005
 */

// COPYRIGHT_BEGIN
//
// The MIT License (MIT)
//
// Copyright (c) 2015 Wizzer Works
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  For information concerning this header file, contact Mark S. Millard,
//  of Wizzer Works at msm@wizzerworks.com.
//
//  More information concerning Wizzer Works may be found at
//
//      http://www.wizzerworks.com
//
// COPYRIGHT_END

#ifndef __MLE_STAGEFUNCS_H_
#define __MLE_STAGEFUNCS_H_

#ifdef MLE_REHEARSAL

/************************************************************

    This file contains strings that help Stages and Sets
    declare what editing features they support at rehearsal
    time. Authoring tools use this information to determine
    what to expose in their UI's.

************************************************************/

// obj type names to use as first arg to getFunctions() 
// and getFunctionAttributes()
#define STAGE_OBJTYPE				"MleStage"
#define SET_OBJTYPE					"MleSet"

// 
// A list of standard function strings for stage editing features.
//

#define STAGE_MOVE_TO_TARGET	    "MleMoveToTarget"
#define STAGE_SNAPPING_TARGET	    "MleSnapTarget"
#define STAGE_BGND_COLOR			"MleBgndColor"
#define STAGE_MULTIPLE_SELECTION    "MleMultiSelect"
#define STAGE_VIEW_ALL				"MleViewAll"

//
// A list of standard function attribute strings for the stage.
//

#define STAGE_ATTR_EDIT_MODES	    "MleEditModes"
#define STAGE_ATTR_VIEWERS			"MleViewers"

#define STAGE_ATTR_EDIT_MODE_PLAY	"MlePlayMode"
#define STAGE_ATTR_EDIT_MODE_PICK	"MlePickMode"
#define STAGE_ATTR_EDIT_MODE_NAV	"MleNavMode"
#define STAGE_ATTR_EDIT_MODE_SEEK	"MleSeekMode"

#define STAGE_ATTR_GLOBAL_RENDER_MODES	"MleGlobalRenderModes"

//
// Standard stage viewers.
//

#define STAGE_VIEWER_EXAMINER	    "Examiner Viewer"
#define STAGE_VIEWER_FLY			"Fly Viewer"
#define STAGE_VIEWER_WALK			"Walk Viewer"
#define STAGE_VIEWER_PLANE			"Plane Viewer"

//
// Standard set function strings.
//

#define SET_LAYERED_ACTORS			"MleLayeredActors"

//
// Standard set function attributes.
//

#define SET_ATTR_RENDER_MODES		"MleRenderModes"

//
// Standard render modes - some supported by each stage.
//

#define RENDER_AS_IS	    "as is"
#define	RENDER_HIDDEN_LINE  "hidden line"
#define	RENDER_NO_TEXTURE   "no textures"
#define	RENDER_LOWRES	    "low res"
#define	RENDER_WIREFRAME    "wireframe"
#define	RENDER_POINTS	    "points"
#define	RENDER_BBOX			"bounding box"
#define RENDER_FULL			"full"
#define RENDER_EDGES	    "edges"
#define RENDER_BBOX_POINTS  "bounding box points"
#define RENDER_BBOX_FACES   "bounding box faces"

#endif	/* MLE_REHEARSAL */

#endif	/* __MLE_STAGEFUNCS_H_ */

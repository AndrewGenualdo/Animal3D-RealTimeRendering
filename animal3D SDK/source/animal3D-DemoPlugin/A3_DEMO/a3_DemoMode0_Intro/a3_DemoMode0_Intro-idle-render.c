/*
	Copyright 2011-2026 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein

	a3_DemoMode0_Intro-idle-render.c
	Demo mode implementations: animation scene.

	********************************************
	*** RENDERING FOR ANIMATION SCENE MODE   ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoMode0_Intro.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_demo_utilities/a3_DemoRenderUtils.h"


// OpenGL
#ifdef _WIN32
#include <gl/glew.h>
#include <Windows.h>
#include <GL/GL.h>
#else	// !_WIN32
#include <OpenGL/gl3.h>
#endif	// _WIN32
#include <stdio.h>


//-----------------------------------------------------------------------------

// controls for pipelines mode
void a3intro_render_controls(a3_DemoState const* demoState, a3_DemoMode0_Intro const* demoMode,
	a3_TextRenderer const* text, a3vec4 const col,
	a3f32 const textAlign, a3f32 const textDepth, a3f32 const textOffsetDelta, a3f32 textOffset)
{
	// forward pipeline names
	a3byte const* renderModeName[intro_renderMode_max] = {
		"Solid color",
		"Texture",
		"Lambert shading",
		"Phong shading",
	};

	a3byte const* bloomPhaseName[intro_phase_max] = {
		"Default",
		"Extract Bright Colors",
		"Blur",
		"HDR (No Bloom)",
		"HDR (With Bloom)"
	};

	// modes
	a3_DemoMode0_Intro_RenderMode const renderMode = demoMode->renderMode;

	a3_DemoMode0_Intro_Phase const bloomPhase = demoMode->bloomPhase;

	// lighting modes
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Rendering mode (%u / %u) ('j' | 'k'): %s", renderMode + 1, intro_renderMode_max, renderModeName[renderMode]);

	//phase count
	a3textDraw(text, textAlign, textOffset += textOffsetDelta, textDepth, col.r, col.g, col.b, col.a,
		"    Bloom Phase (%u / %u) ('6' | '7'): %s", bloomPhase, intro_phase_max - 1, bloomPhaseName[bloomPhase]);
}


//-----------------------------------------------------------------------------

// sub-routine for rendering the demo state using the shading pipeline
void a3intro_render(a3_DemoState const* demoState, a3_DemoMode0_Intro const* demoMode, a3f64 const dt)
{
	// pointers
	const a3_DemoStateShaderProgram* currentDemoProgram;

	// indices
	a3ui32 i = 0, j = 0;

	// RGB
	const a3vec4 rgba4[] = {
		{ 40.00f, 0.00f, 0.00f, 1.00f },	// red
		{ 1.00f, 0.25f, 0.00f, 1.00f },
		{ 1.00f, 0.50f, 0.00f, 1.00f },	// orange
		{ 1.00f, 0.75f, 0.00f, 1.00f },
		{ 1.00f, 1.00f, 0.00f, 1.00f },	// yellow
		{ 0.75f, 1.00f, 0.00f, 1.00f },
		{ 20.0f, 40.00f, 0.00f, 1.00f },	// lime
		{ 0.25f, 1.00f, 0.00f, 1.00f },
		{ 0.00f, 1.00f, 0.00f, 1.00f },	// green
		{ 0.00f, 1.00f, 0.25f, 1.00f },
		{ 0.00f, 1.00f, 0.50f, 1.00f },	// aqua
		{ 0.00f, 1.00f, 0.75f, 1.00f },
		{ 0.00f, 40.00f, 40.00f, 1.00f },	// cyan
		{ 0.00f, 0.75f, 1.00f, 1.00f },
		{ 0.00f, 0.50f, 1.00f, 1.00f },	// sky
		{ 0.00f, 0.25f, 1.00f, 1.00f },
		{ 0.00f, 0.00f, 1.00f, 1.00f },	// blue
		{ 0.25f, 0.00f, 1.00f, 1.00f },
		{ 0.50f, 0.00f, 1.00f, 1.00f },	// purple
		{ 0.75f, 0.00f, 1.00f, 1.00f },
		{ 1.00f, 0.00f, 1.00f, 1.00f },	// magenta
		{ 1.00f, 0.00f, 0.75f, 1.00f },
		{ 1.00f, 0.00f, 0.50f, 1.00f },	// rose
		{ 1.00f, 0.00f, 0.25f, 1.00f },
	};
	const a3vec4 grey4[] = {
		{ 0.5f, 0.5f, 0.5f, 1.0f },	// solid grey
		{ 0.5f, 0.5f, 0.5f, 0.5f },	// translucent grey
	};
	const a3ui32 hueCount = sizeof(rgba4) / sizeof(*rgba4),
		redIndex = 0, orangeIndex = 2, yellowIndex = 4, limeIndex = 6,
		greenIndex = 8, aquaIndex = 10, cyanIndex = 12, skyIndex = 14,
		blueIndex = 16, purpleIndex = 18, magentaIndex = 20, roseIndex = 22;
	const a3real
		* const red = rgba4[redIndex].v, * const orange = rgba4[orangeIndex].v, * const yellow = rgba4[yellowIndex].v, * const lime = rgba4[limeIndex].v,
		* const green = rgba4[greenIndex].v, * const aqua = rgba4[aquaIndex].v, * const cyan = rgba4[cyanIndex].v, * const sky = rgba4[skyIndex].v,
		* const blue = rgba4[blueIndex].v, * const purple = rgba4[purpleIndex].v, * const magenta = rgba4[magentaIndex].v, * const rose = rgba4[roseIndex].v,
		* const grey = grey4[0].v, * const grey_t = grey4[1].v;

	// camera used for drawing
	const a3_ProjectorComponent* activeCamera = demoMode->projector;
	const a3_SceneObjectComponent* activeCameraObject = activeCamera->sceneObjectPtr;
	const a3_SceneObjectComponent* currentSceneObject, * endSceneObject;

	// temp drawable pointers
	const a3_VertexDrawable* drawable[] = {
		0,								// root
		0,								// camera
		demoState->draw_unit_box,		// skybox
		demoState->draw_unit_sphere,	// objects
		demoState->draw_unit_cylinder,
		demoState->draw_unit_capsule,
		demoState->draw_unit_torus,
		demoState->draw_unit_cone,
		demoState->draw_teapot,
		demoState->draw_unit_plane_z,
	};

	// temp texture pointers
	const a3_Texture* texture_dm[] = {
		0,								// root
		0,								// camera
		demoState->tex_skybox_clouds,	// skybox
		demoState->tex_checker,			// objects
		demoState->tex_checker,
		demoState->tex_checker,
		demoState->tex_checker,
		demoState->tex_checker,
		demoState->tex_checker,
		demoState->tex_checker,
	};

	// forward pipeline shader programs
	const a3_DemoStateShaderProgram* renderProgram[intro_renderMode_max] = {
		demoState->prog_drawColorUnif,
		demoState->prog_drawTexture,
		demoState->prog_drawLambert,
		demoState->prog_drawPhong,
	};

	// target info
	a3_DemoMode0_Intro_RenderMode const renderMode = demoMode->renderMode;
	a3_DemoMode0_Intro_Phase const bloomPhase = demoMode->bloomPhase;

	// final model matrix and full matrix stack
	a3mat4 projectionMat = activeCamera->projectorMatrixStackPtr->projectionMat;
	a3mat4 projectionMatInv = activeCamera->projectorMatrixStackPtr->projectionMatInverse;
	a3mat4 viewProjectionMat = activeCamera->projectorMatrixStackPtr->viewProjectionMat;
	a3mat4 modelMat, modelViewMat, modelViewProjectionMat, atlasMat;


	//-------------------------------------------------------------------------
	// 1) SCENE PASS: render scene with desired shader
	//	- activate scene framebuffer
	//	- draw scene
	//		- clear buffers
	//		- render shapes using appropriate shaders
	//		- capture color and depth

	// reset viewport
	a3framebufferDeactivateSetViewport(a3fbo_depth24_stencil8,
		-demoState->frameBorder, -demoState->frameBorder, demoState->frameWidth, demoState->frameHeight);

	// ****TO-DO: 
	//	-> configure stereo viewport
	//
	// | YOUR CODE HERE |
	// V                V
	//-----------------------------------------------------------------------------
	{

	}
	//-----------------------------------------------------------------------------
	// ^ YOUR CODE HERE ^

	// clear buffers
	if (demoState->displaySkybox)
	{
		// skybox clear: just draw skybox
		glDepthFunc(GL_ALWAYS);
		modelMat = demoMode->obj_skybox->modelMatrixStackPtr->modelMat;
		atlasMat = demoMode->obj_skybox->modelMatrixStackPtr->atlasMat;
		a3demo_drawModelTexturedColored_invertModel(
			modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, atlasMat.m,
			demoState->prog_drawTexture, demoState->draw_unit_box, demoState->tex_skybox_clouds, a3vec4_one.v);
		glDepthFunc(GL_LEQUAL);
	}
	else
	{
		// full clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	// stencil test
	//if (demoState->stencilTest)
	//	a3demo_drawStencilTest(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, demoState->prog_drawColorUnif, demoState->draw_unit_sphere);

	// select program based on settings
	currentDemoProgram = renderProgram[renderMode];
	a3shaderProgramActivate(currentDemoProgram->program);

	// send shared data: 
	//	- projection matrix
	//	- light data
	//	- activate shared textures including atlases if using
	//	- shared animation data
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP, 1, projectionMat.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP_inv, 1, projectionMatInv.mm);
	a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uAtlas, 1, a3mat4_identity.mm);
	a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor0, hueCount, rgba4->v);
	if (demoState->updateAnimation)
		a3shaderUniformSendDouble(a3unif_single, currentDemoProgram->uTime, 1, &demoState->timer_display->totalTime);

	// ****DONE: 
	//	-> send lighting uniforms and bind blocks where appropriate


	// ****TO-DO: 
	//	-> recalculate projection matrices for stereo
	//	-> send stereo projection matrices as uniform
	//
	// | YOUR CODE HERE |
	// V                V
	//-----------------------------------------------------------------------------
	{

	}
	//-----------------------------------------------------------------------------
	// ^ YOUR CODE HERE ^

	// select pipeline algorithm
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	a3framebufferActivate(bloomPhase > intro_phaseNone ? demoState->fbo_hdr : NULL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	// forward shading algorithms
	for (currentSceneObject = demoMode->obj_sphere, endSceneObject = demoMode->obj_ground;
		currentSceneObject <= endSceneObject; ++currentSceneObject)
	{
		j = currentSceneObject->sceneHierarchyIndex;
		i = (j * 3 + hueCount / 2) % hueCount;
		switch (renderMode)
		{
		case intro_renderModePhong:
			// activate specular map, fall through to Lambert
			a3textureActivate(texture_dm[j], a3tex_unit01);
		case intro_renderModeLambert:
			// send lights and matrices, fall through to texturing
			modelViewMat = currentSceneObject->modelMatrixStackPtr->modelViewMat;
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMV, 1, modelViewMat.mm);
			modelViewMat = currentSceneObject->modelMatrixStackPtr->modelViewMatInverseTranspose;
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMV_nrm, 1, modelViewMat.mm);
		case intro_renderModeTexture:
			// activate diffuse map, fall through to solid color
			a3textureActivate(texture_dm[j], a3tex_unit00);
		case intro_renderModeSolid:
			// send general matrix and color, end
			modelViewProjectionMat = currentSceneObject->modelMatrixStackPtr->modelViewProjectionMat;
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, rgba4[i].v);
			break;
		}
		a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uIndex, 1, &j);
		a3vertexDrawableActivateAndRender(drawable[j]);
	}
	
	a3ui32 blurPasses = 10;
	a3framebufferDeactivate();
	// render fullscreen quad to capture bright areas
	if (bloomPhase >= intro_phaseExtract) 
	{
		a3framebufferActivate(bloomPhase > intro_phaseExtract ? demoState->fbo_brightness : NULL);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentDemoProgram = demoState->prog_postBright;
		a3shaderProgramActivate(currentDemoProgram->program);

		//bind scene texture
		a3framebufferBindColorTexture(demoState->fbo_hdr, a3tex_unit00, 0);

		/*glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
		glEnd();*/
		a3vertexDrawableActivateAndRender(demoState->draw_fsq);

		a3framebufferDeactivate();
	}

	//blur
	if (bloomPhase >= intro_phaseBlur) 
	{
		
		currentDemoProgram = demoState->prog_postBlur;
		a3shaderProgramActivate(currentDemoProgram->program);

		for (i = 0; i < blurPasses; i++)
		{
			a3framebufferActivate((bloomPhase == intro_phaseBlur && i == blurPasses - 1) ? NULL : &demoState->fbo_bloomPingPong[(i + 1) % 2]);
			a3shaderProgramActivate(currentDemoProgram->program);

			

			//set horizontal uniform
			a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uIndex, 1, (int[]) { (i + 1) % 2 });

			a3framebufferBindColorTexture(i == 0 ? demoState->fbo_brightness : &demoState->fbo_bloomPingPong[i % 2], a3tex_unit00, 0);

			glBegin(GL_QUADS);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
			glEnd();
			a3framebufferDeactivate();
		}
	}

	//hdr + bloom
	if (bloomPhase >= intro_phaseHDR) 
	{
		a3framebufferActivate(NULL);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentDemoProgram = demoState->prog_postBlend;
		a3shaderProgramActivate(currentDemoProgram->program);

		a3framebufferBindColorTexture(demoState->fbo_hdr, a3tex_unit00, 0);
		a3framebufferBindColorTexture(&demoState->fbo_bloomPingPong[blurPasses % 2], a3tex_unit01, 0);

		a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uIndex, 1, (int[]) { bloomPhase == intro_enableBloom ? 1 : 0 });

		glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
		glEnd();
		a3framebufferDeactivate();
	}

	// stop using stencil
	if (demoState->stencilTest)
		glDisable(GL_STENCIL_TEST);


	//-------------------------------------------------------------------------
	// OVERLAYS: done after FSQ so they appear over everything else
	//	- disable depth testing
	//	- draw overlays appropriately

	// enable alpha
	a3demo_enableCompositeBlending();

	// scene overlays
	if (demoState->displayGrid || demoState->displayTangentBases || demoState->displayWireframe)
	{
		// draw grid aligned to world
		if (demoState->displayGrid)
		{
			a3demo_drawModelSolidColor(modelViewProjectionMat.m, viewProjectionMat.m, a3mat4_identity.m, demoState->prog_drawColorUnif, demoState->draw_grid, blue);
		}

		if (demoState->displayTangentBases || demoState->displayWireframe)
		{
			const a3i32 flag[1] = { demoState->displayTangentBases * 3 + demoState->displayWireframe * 4 };
			const a3f32 size[1] = { 0.0625f };

			currentDemoProgram = demoState->prog_drawTangentBasis;

			a3shaderProgramActivate(currentDemoProgram->program);

			// projection matrix
			a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uP, 1, projectionMat.mm);
			// wireframe color
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor0, hueCount, rgba4->v);
			// blend color
			a3shaderUniformSendFloat(a3unif_vec4, currentDemoProgram->uColor, 1, a3vec4_one.v);
			// tangent basis size
			a3shaderUniformSendFloat(a3unif_single, currentDemoProgram->uSize, 1, size);
			// overlay flag
			a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uFlag, 1, flag);

			for (currentSceneObject = demoMode->obj_sphere, endSceneObject = demoMode->obj_ground;
				currentSceneObject <= endSceneObject; ++currentSceneObject)
			{
				j = currentSceneObject->sceneHierarchyIndex;
				i = (j * 3) % hueCount;
				modelViewMat = currentSceneObject->modelMatrixStackPtr->modelViewMat;
				a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMV, 1, modelViewMat.mm);
				modelViewMat = currentSceneObject->modelMatrixStackPtr->modelViewMatInverseTranspose;
				a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMV_nrm, 1, modelViewMat.mm);
				modelViewProjectionMat = currentSceneObject->modelMatrixStackPtr->modelViewProjectionMat;
				a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, modelViewProjectionMat.mm);
				a3shaderUniformSendInt(a3unif_single, currentDemoProgram->uIndex, 1, &i);
				a3vertexDrawableActivateAndRender(drawable[j]);
			}
		}
	}

	// overlays with no depth
	glDisable(GL_DEPTH_TEST);
	

	// hidden volumes
	if (demoState->displayHiddenVolumes)
	{

	}

	
	

	// superimpose axes
	// draw coordinate axes in front of everything
	currentDemoProgram = demoState->prog_drawColorAttrib;
	a3shaderProgramActivate(currentDemoProgram->program);
	a3vertexDrawableActivate(demoState->draw_axes);

	// center of world from current viewer
	// also draw other viewer/viewer-like object in scene
	if (demoState->displayWorldAxes)
	{
		a3shaderUniformSendFloatMat(a3unif_mat4, 0, currentDemoProgram->uMVP, 1, viewProjectionMat.mm);
		a3vertexDrawableRenderActive();
	}

	// individual objects (based on scene graph)
	if (demoState->displayObjectAxes)
	{
		for (currentSceneObject = demoMode->obj_sphere, endSceneObject = demoMode->obj_ground;
			currentSceneObject <= endSceneObject; ++currentSceneObject)
		{
			j = currentSceneObject->sceneHierarchyIndex;
			modelMat = currentSceneObject->modelMatrixStackPtr->modelMat;
			a3demo_drawModelSimple(modelViewProjectionMat.m, viewProjectionMat.m, modelMat.m, currentDemoProgram);
		}
	}
}


//-----------------------------------------------------------------------------

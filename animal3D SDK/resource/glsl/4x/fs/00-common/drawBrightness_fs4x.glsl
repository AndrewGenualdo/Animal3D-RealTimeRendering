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
	
	drawTexture_fs4x.glsl
	Output texture blended with color.
*/

#version 450

uniform sampler2D uImage00;
uniform int uIndex; //bool for horizontal (true = 1) or vertical (false = 0)


layout (location = 1) out vec4 FragColor;
layout (location = 0) out vec4 BrightColor;

in vec4 vTexcoord_atlas;

//based on https://learnopengl.com/Advanced-Lighting/Bloom

void main()
{
	vec4 result = texture(uImage00, vTexcoord_atlas.xy);
	FragColor = result;

	// check whether result is higher than some threshold, if so, output as bloom threshold color
	float brightness = dot(result.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

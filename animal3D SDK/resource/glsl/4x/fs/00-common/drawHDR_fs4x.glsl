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

uniform sampler2D uImage00; //scene
uniform sampler2D uImage01; //bloom

uniform float uExposure = 1.0f;
uniform int uIndex = 1; //bloom toggle (1 = enabled, 0 = disabled)

in vec4 vTexcoord_atlas;
out vec4 FragColor;

void main()
{
	//FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	float gamma = 2.2;
    vec3 hdrColor = texture(uImage00, vTexcoord_atlas.xy).rgb;
	vec3 bloomColor = texture(uImage01, vTexcoord_atlas.xy).rgb;

	if(uIndex == 1) hdrColor += bloomColor;

	//tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * uExposure);

    //gamma correction
    result = pow(result, vec3(1.0 / gamma));

    FragColor = vec4(result, 1.0);
	//FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
	//FragColor = vec4(hdrColor, 1.0);
}

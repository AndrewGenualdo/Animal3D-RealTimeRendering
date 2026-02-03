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


out vec4 FragColor;

in vec4 vTexcoord_atlas;


const float[5] weights = {0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162};

//based on https://learnopengl.com/Advanced-Lighting/Bloom

void main()
{
	vec2 texelSize = 1.0 / textureSize(uImage00, 0);

    vec3 result = texture(uImage00, vTexcoord_atlas.xy).rgb * weights[0];

	if(uIndex == 1) 
	{
		for(int i = 1; i < 5; ++i)
        {
            result += texture(uImage00, vTexcoord_atlas.xy + vec2(texelSize.x * i, 0.0)).rgb * weights[i];
            result += texture(uImage00, vTexcoord_atlas.xy - vec2(texelSize.x * i, 0.0)).rgb * weights[i];
        }
	}
	else 
	{
		for(int i = 1; i < 5; ++i)
        {
            result += texture(uImage00, vTexcoord_atlas.xy + vec2(0.0, texelSize.y * i)).rgb * weights[i];
            result += texture(uImage00, vTexcoord_atlas.xy - vec2(0.0, texelSize.y * i)).rgb * weights[i];
        }
	}
    

    FragColor = vec4(result, 1.0);
	//FragColor = vec4(vTexcoord_atlas.x, vTexcoord_atlas.y, 0, 1.0f);
	//FragColor = texture(uImage00, vTexcoord_atlas.xy);
}

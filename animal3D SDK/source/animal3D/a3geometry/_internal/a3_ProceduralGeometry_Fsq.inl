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

	a3_ProceduralGeometry_Box.inl
	Internal generation functions for Box.

	**DO NOT MODIFY THIS FILE**
*/

#ifdef __ANIMAL3D_PROCEDURALGEOMETRY_C


a3ret a3proceduralCreateDescriptorFsq(a3_ProceduralGeometryDescriptor* geom_out, const a3_ProceduralGeometryFlag flags)
{
	a3_ProceduralGeometryDescriptor ret = { 0 };
	if (geom_out && !geom_out->shape)
	{
		return a3proceduralSetDescriptorFlags(geom_out, flags);
	}
	return -1;
}


a3ret a3proceduralInternalGenerateFsq(a3_GeometryData* geomData, const a3_ProceduralGeometryDescriptor* geom)
{
	const a3_ProceduralGeometryFlag flags = a3proceduralGetDescriptorFlags(geom);

	const a3ui32 numVertices = geomData->numVertices = 4;
	//geomData->numVertices = 4;
	const a3ui32 numIndices = geomData->numIndices = 6;
	//geomData->numIndices = 6;
	const a3ui32 vertexSize = a3proceduralInternalCreateFormats(geomData->vertexFormat, geomData->indexFormat, numVertices, numIndices, flags, 0);
	const a3ui32 indexSize = geomData->indexFormat->indexSize;

	// allocate space for whole buffer required
	const a3ui32 vertexBufferSize = a3vertexFormatGetStorageSpaceRequired(geomData->vertexFormat, numVertices);
	const a3ui32 indexBufferSize = a3indexFormatGetStorageSpaceRequired(geomData->indexFormat, numIndices);
	a3ubyte* const data = (a3ubyte*)malloc(vertexBufferSize + indexBufferSize);

	a3f32* const positions = (a3f32*)(data),
		* const texcoords = positions + geomData->vertexFormat->attribElements[a3attrib_position] * numVertices,
		* const vertexDataEnd = texcoords + geomData->vertexFormat->attribElements[a3attrib_texcoord] * numVertices;


	a3ubyte* const vertexDataEndConfirm = data + vertexBufferSize;
	a3ubyte* const indices = vertexDataEndConfirm,
		* const indexDataEnd = indices + indexSize * numIndices;

	positions[0] = -0.5f;
	positions[1] = -0.5f;
	positions[2] = 0.0f;
	
	positions[3] = 0.5f;
	positions[4] = -0.5f;
	positions[5] = 0.0f;

	positions[6] = 0.5f;
	positions[7] = 0.5f;
	positions[8] = 0.0f;

	positions[9] = -0.5f;
	positions[10] = 0.5f;
	positions[11] = 0.0f;

	texcoords[0] = 0.0f;
	texcoords[1] = 0.0f;

	texcoords[2] = 1.0f;
	texcoords[3] = 0.0f;
	
	texcoords[4] = 1.0f;
	texcoords[5] = 1.0f;

	texcoords[6] = 0.0f;
	texcoords[7] = 1.0f;

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;


	geomData->attribData[a3attrib_geomPosition] = positions;
	geomData->attribData[a3attrib_geomTexcoord] = texcoords;
	geomData->indexData = numIndices ? indices : 0;
	geomData->data = data;
	geomData->primType = a3prim_triangles;

	//-------------------------------------------------------------------------
	// done
	return 1;
}


#endif	// __ANIMAL3D_PROCEDURALGEOMETRY_C
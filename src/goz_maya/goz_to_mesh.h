#pragma once

#include<goz/goz_core.h>

#include <cstdint>
#include <numeric>

#include <maya/MPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MIntArray.h>
#include <maya/MObject.h>
#include <maya/MFnSet.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnMesh.h>



class goz_to_mesh
{
public:
	static MStatus build(const goz_data* goz_data);

private:
	explicit goz_to_mesh(const goz_data* goz_data);
	MStatus create_mesh(MFnMesh& mesh_fn, MObject& return_object);

	void get_vertex_array();
	void get_polygon_counts();
	void get_face_vertex_indices();
	void get_uvs();

	const goz_data* m_goz_data;

	MFloatArray m_u_array;
	MFloatArray m_v_array;
	MPointArray m_vertex_array;

	MIntArray m_polygon_counts;
	MIntArray m_face_vertex_indices;

	uint32_t m_face_vertex_count = 0;
};

#pragma once

#include<goz/goz_core.h>

#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MStatus.h>
#include <maya/MDagPath.h>
#include <maya/MFloatPointArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MIntArray.h>
#include <maya/MGlobal.h>



class mesh_to_goz
{
public:
	static MStatus create(const MString& name, const MDagPath& mesh_path, goz_data* goz_data);

private:
	explicit mesh_to_goz(const MDagPath& mesh_path, goz_data* goz_data);

	[[nodiscard]] MStatus get_vertices() const;
	[[nodiscard]] MStatus get_polygons() const;
	[[nodiscard]] MStatus get_uvs() const;

	MDagPath m_mesh_path;
	goz_data* m_goz_data;
};

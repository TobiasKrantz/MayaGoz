#include "mesh_to_goz.h"

mesh_to_goz::mesh_to_goz(const MDagPath& mesh_path, goz_data* goz_data) : m_mesh_path(mesh_path), m_goz_data(goz_data)
{
}

MStatus mesh_to_goz::create(const MString& name, const MDagPath& mesh_path, goz_data* goz_data)
{
	const mesh_to_goz writer(mesh_path, goz_data);

	writer.m_goz_data->name = name.asChar();

	MStatus status = writer.get_vertices();
	if (status.error()) return status;

	status = writer.get_polygons();
	if (status.error()) return status;

	status = writer.get_uvs();
	if (status.error()) return status;

	return status;
}

MStatus mesh_to_goz::get_vertices() const
{
	MFloatPointArray mesh_points;
	MStatus status;

	const MFnMesh mesh_fn(m_mesh_path, &status);
	if (status.error()) return status;

	status = mesh_fn.getPoints(mesh_points, MSpace::kWorld);
	if (status.error()) return status;

	m_goz_data->vertex_coordinates.reserve(mesh_fn.numVertices());


	for (const auto& point : mesh_points)
	{
		vertex vertex;
		vertex.x = point.x;
		vertex.y = point.y;
		vertex.z = point.z;

		m_goz_data->vertex_coordinates.push_back(vertex);
	}


	return status;
}

MStatus mesh_to_goz::get_polygons() const
{
	MStatus status;
	MIntArray vertex_indices;

	MItMeshPolygon poly_iterator(m_mesh_path, MObject::kNullObj, &status);
	m_goz_data->polygon_indices.reserve(poly_iterator.count());

	while (!poly_iterator.isDone())
	{
		const unsigned int vertex_count = poly_iterator.polygonVertexCount(&status);
		if (status.error()) return status;

		if (vertex_count != 3 && vertex_count != 4)
		{
			MGlobal::displayError("Invalid Mesh: Only Quad and Triangle Meshes Supported");
			return MStatus::kFailure;
		}

		status = poly_iterator.getVertices(vertex_indices);
		if (status.error()) return status;

		polygon polygon;
		polygon.v1 = vertex_indices[0];
		polygon.v2 = vertex_indices[1];
		polygon.v3 = vertex_indices[2];
		polygon.v4 = (vertex_count == 4) ? vertex_indices[3] : UINT32_MAX;

		m_goz_data->polygon_indices.push_back(polygon);

		poly_iterator.next();
	}

	return status;
}

MStatus mesh_to_goz::get_uvs() const
{
	MStatus status;
	float2 uv;

	MItMeshPolygon poly_iterator(m_mesh_path, MObject::kNullObj, &status);
	m_goz_data->uv_data.reserve(poly_iterator.count());

	if (status.error()) return status;

	while (!poly_iterator.isDone())
	{
		polygon_uv poly_uv;

		status = poly_iterator.getUV(0, uv);
		if (status.error()) return status;

		poly_uv.uv1.u = uv[0];
		poly_uv.uv1.v = uv[1];

		status = poly_iterator.getUV(1, uv);
		if (status.error()) return status;

		poly_uv.uv2.u = uv[0];
		poly_uv.uv2.v = uv[1];

		status = poly_iterator.getUV(2, uv);
		if (status.error()) return status;

		poly_uv.uv3.u = uv[0];
		poly_uv.uv3.v = uv[1];

		if (poly_iterator.polygonVertexCount() == 4)

		{
			status = poly_iterator.getUV(3, uv);
			if (status.error()) return status;

			poly_uv.uv4.u = uv[0];
			poly_uv.uv4.v = uv[1];
		}

		m_goz_data->uv_data.push_back(poly_uv);

		poly_iterator.next();
	}

	return status;
}

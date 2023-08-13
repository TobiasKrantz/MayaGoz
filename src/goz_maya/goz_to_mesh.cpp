#include "goz_to_mesh.h"


MStatus assign_default_shader(const MObject& mesh_object)
{
	MFnSet set_fn;
	MObject shader_object;
	MSelectionList selection_list;

	MStatus status = selection_list.add("initialShadingGroup");
	if (status.error()) return status;

	status = selection_list.getDependNode(0, shader_object);
	if (status.error()) return status;

	status = set_fn.setObject(shader_object);
	if (status.error()) return status;

	status = set_fn.addMember(mesh_object);
	if (status.error()) return status;

	return status;
}

goz_to_mesh::goz_to_mesh(const goz_data* goz_data) : m_goz_data(goz_data)
{
}

MStatus goz_to_mesh::build(const goz_data* goz_data)
{
	MObject mesh_object;
	MFnMesh mesh_fn;
	MStatus status;

	goz_to_mesh builder(goz_data);

	builder.get_vertex_array();
	builder.get_polygon_counts();
	builder.get_face_vertex_indices();

	status = builder.create_mesh(mesh_fn, mesh_object);
	if (status.error()) return status;

	MFnDependencyNode dependency_node(mesh_object, &status);
	if (status.error()) return status;

	dependency_node.setName(goz_data->name.c_str(), false, &status);
	if (status.error()) return status;

	status = assign_default_shader(mesh_object);
	if (status.error()) return status;

	return status;
}

MStatus goz_to_mesh::create_mesh(MFnMesh& mesh_fn, MObject& return_object)
{
	MStatus status;

	if (m_goz_data->uv_data.empty())
	{
		return_object = mesh_fn.create(
			m_goz_data->vertex_count,
			m_goz_data->polygon_count,
			m_vertex_array,
			m_polygon_counts,
			m_face_vertex_indices,
			MObject::kNullObj,
			&status
		);
	}

	else
	{
		// has uvs
		get_uvs();

		return_object = mesh_fn.create(
			m_goz_data->vertex_count,
			m_goz_data->polygon_count,
			m_vertex_array,
			m_polygon_counts,
			m_face_vertex_indices,
			m_u_array,
			m_v_array,
			MObject::kNullObj,
			&status
		);

		if (status.error()) return status;

		MIntArray uv_indices(m_face_vertex_count);
		std::iota(uv_indices.begin(), uv_indices.end(), 0);

		status = mesh_fn.assignUVs(m_polygon_counts, uv_indices);
	}

	return status;
}

void goz_to_mesh::get_vertex_array()
{
	m_vertex_array = MPointArray(m_goz_data->vertex_count);

	for (unsigned int i = 0; i < m_goz_data->vertex_count; i++)
	{
		m_vertex_array[i].x = m_goz_data->vertex_coordinates[i].x;
		m_vertex_array[i].y = m_goz_data->vertex_coordinates[i].y;
		m_vertex_array[i].z = m_goz_data->vertex_coordinates[i].z;
	}
}

void goz_to_mesh::get_polygon_counts()
{
	m_polygon_counts = MIntArray(m_goz_data->polygon_count, 4);
	m_face_vertex_count = m_goz_data->polygon_count * 4;
	uint8_t is_triangle = 0;

	for (unsigned int i = 0; i < m_goz_data->polygon_count; i++)
	{
		is_triangle = (m_goz_data->polygon_indices[i].v4 == UINT32_MAX);
		m_polygon_counts[i] -= is_triangle;
		m_face_vertex_count -= is_triangle;
	}
}

void goz_to_mesh::get_face_vertex_indices()
{
	m_face_vertex_indices = MIntArray(m_face_vertex_count);
	auto poly_iterator = m_face_vertex_indices.begin();

	for (unsigned int i = 0; i < m_goz_data->polygon_count; i++)
	{
		*poly_iterator = m_goz_data->polygon_indices[i].v1;
		++poly_iterator;
		*poly_iterator = m_goz_data->polygon_indices[i].v2;
		++poly_iterator;
		*poly_iterator = m_goz_data->polygon_indices[i].v3;
		++poly_iterator;

		if (m_goz_data->polygon_indices[i].v4 == UINT32_MAX) continue; // If it's a triangle, go next

		*poly_iterator = m_goz_data->polygon_indices[i].v4;
		++poly_iterator;
	}
}

void goz_to_mesh::get_uvs()
{
	m_u_array = MFloatArray(m_face_vertex_count, 0.0f);
	m_v_array = MFloatArray(m_face_vertex_count, 0.0f);

	auto u_iterator = m_u_array.begin();
	auto v_iterator = m_v_array.begin();

	for (unsigned int i = 0; i < m_goz_data->polygon_count; i++)
	{
		*u_iterator = m_goz_data->uv_data[i].uv1.u;
		++u_iterator;

		*u_iterator = m_goz_data->uv_data[i].uv2.u;
		++u_iterator;

		*u_iterator = m_goz_data->uv_data[i].uv3.u;
		++u_iterator;

		if (m_goz_data->polygon_indices[i].v4 == UINT32_MAX) continue; // If it's a triangle, go next

		*u_iterator = m_goz_data->uv_data[i].uv4.u;
		++u_iterator;
	}


	for (unsigned int i = 0; i < m_goz_data->polygon_count; i++)
	{
		*v_iterator = m_goz_data->uv_data[i].uv1.v;
		++v_iterator;

		*v_iterator = m_goz_data->uv_data[i].uv2.v;
		++v_iterator;

		*v_iterator = m_goz_data->uv_data[i].uv3.v;
		++v_iterator;

		if (m_goz_data->polygon_indices[i].v4 == UINT32_MAX) continue; // If it's a triangle, go next

		*v_iterator = m_goz_data->uv_data[i].uv4.v;
		++v_iterator;
	}
}

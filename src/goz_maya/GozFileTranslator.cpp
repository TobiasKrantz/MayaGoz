#pragma warning (disable:26812)

#include "GozFileTranslator.h"

const char* GozFileTranslator::goz_export_default_options = "";
const char* GozFileTranslator::goz_import_default_options = "";
const char* GozFileTranslator::goz_import_name = "gozImport";
const char* GozFileTranslator::goz_export_name = "gozExport";

bool GozFileTranslator::haveReadMethod() const {
	return true;
}

bool GozFileTranslator::haveWriteMethod() const {
	return true;
}

MString GozFileTranslator::defaultExtension() const {
	return "goz";
}

MString GozFileTranslator::filter() const {
	return "*.goz";
}

bool GozFileTranslator::haveNamespaceSupport() const {
	return true;
}

MStatus GozFileTranslator::writer(const MFileObject& file, const MString& options_string, MPxFileTranslator::FileAccessMode mode) {
	MStatus status = MStatus::kSuccess;
	const MString name = file.resolvedFullName();

	goz_data data;
	MSelectionList selection_list;
	MGlobal::getActiveSelectionList(selection_list);
	MDagPath mesh_path;

	if (selection_list.isEmpty()) {
		return status;
	}

	selection_list.getDagPath(0, mesh_path);

	if (!mesh_path.hasFn(MFn::kTransform)) return MStatus::kFailure;

	MStringArray split_string;
	const MString mesh_name = mesh_path.partialPathName();
	mesh_name.split('|', split_string);

	// Get first shape
	status = mesh_path.extendToShapeDirectlyBelow(0);
	if (status.error()) return status;

	if (!mesh_path.hasFn(MFn::kMesh)) return MStatus::kFailure;

	status = mesh_to_goz::create(split_string[0], mesh_path, &data);
	if (status.error()) return status;

	try {
		goz_writer::write(name.asWChar(), &data);
	}

	catch (const std::exception& exception) {
		MGlobal::displayError(exception.what());
		return MStatus::kFailure;
	}

	return MStatus::kSuccess;
}

MPxFileTranslator::MFileKind GozFileTranslator::identifyFile(const MFileObject& file, const char* buffer, short size) const {
	const MString name = file.resolvedName();

	const int len = name.numChars();

	if (len > 4 && name.substringW(len - 4, len - 1).toLowerCase() == ".goz")
	{
		return kIsMyFileType;
	}
	return kNotMyFileType;
}

MStatus GozFileTranslator::reader(const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode) {
	const MString file_path = file.resolvedFullName();
	goz_data goz_data;

	try {
		goz_data = goz_parser::parse(file_path.asWChar());
	}

	catch (const std::exception& exception) {
		MGlobal::displayError(exception.what());
		return MStatus::kFailure;
	}

	goz_to_mesh::build(&goz_data);

	return MStatus::kSuccess;
}
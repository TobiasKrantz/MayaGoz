#pragma once

#include <maya/MPxFileTranslator.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>

#include "goz/goz_core.h"
#include "goz/goz_writer.h"
#include "goz/goz_parser.h"

#include "goz_to_mesh.h"
#include "mesh_to_goz.h"


class GozFileTranslator : public MPxFileTranslator
{
public:
	static const char* goz_export_default_options;
	static const char* goz_import_default_options;
	static const char* goz_import_name;
	static const char* goz_export_name;

    static void* creator() { return new GozFileTranslator; }

	MStatus		reader(const MFileObject& file, const MString& optionsString, FileAccessMode mode) override;
	MStatus		writer(const MFileObject& file, const MString& options_string, FileAccessMode mode) override;

	[[nodiscard]] bool		haveReadMethod() const override;
	[[nodiscard]] bool		haveWriteMethod() const override;
	[[nodiscard]] bool		haveNamespaceSupport() const override;

	[[nodiscard]] MString     defaultExtension() const override;
	[[nodiscard]] MString     filter() const override;

	MPxFileTranslator::MFileKind identifyFile(const MFileObject& file, const char* buffer, short size) const override;
};

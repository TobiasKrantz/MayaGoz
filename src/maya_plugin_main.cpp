#include <goz_maya/GozFileTranslator.h>

#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>


MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugIn(obj, "Tobias Krantz", "1.0", "Any");

	status = plugIn.registerFileTranslator(
		GozFileTranslator::goz_import_name, //Translator name
		NULL, // Pixmap name
		GozFileTranslator::creator,
		NULL, // Options Script name
		GozFileTranslator::goz_import_default_options,
		false);

	if (!status)
	{
		cout << "Unable to register File Translator " << std::endl;
		return status;
	}

	status = plugIn.registerFileTranslator(
		GozFileTranslator::goz_export_name, //Translator name
		NULL, // Pixmap name
		GozFileTranslator::creator,
		NULL, // Options Script name
		GozFileTranslator::goz_export_default_options, // DefaultOptiosString
		false); // RequiresMel

	if (!status)
	{
		cout << "Unable to register File Translator " << std::endl;
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugIn(obj);

	status = plugIn.deregisterFileTranslator(GozFileTranslator::goz_import_name);
	if (!status)
	{
		cout << "Unable to deregister File Translator " << std::endl;
		return status;
	}


	status = plugIn.deregisterFileTranslator(GozFileTranslator::goz_export_name);

	if (!status)
	{
		cout << "Unable to deregister File Translator " << std::endl;
		return status;
	}

	return status;
}

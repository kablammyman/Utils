#pragma once
#include "DatabaseController.h"
class BaseDBInterface
{
public:
	BaseDBInterface() {}
	BaseDBInterface(DatabaseController *db)
	{
		SetDBController(db);
	}
	void SetDBController(DatabaseController *db)
	{
		dbController = db;
	}
	
	DatabaseController::Error lastError;
	std::string GetLastErrorMessage() { return lastError.errorMessage; }
protected:
		DatabaseController *dbController;
};
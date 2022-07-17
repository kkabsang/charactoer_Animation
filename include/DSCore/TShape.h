#pragma once
#include "TModel.h"
class TShape : public TModel
{
};
class DPlaneShape : public TShape
{
public:
	bool CreateVertexData()override;
	bool CreateIndexData()override;
};
class TBoxShape : public TShape
{
};

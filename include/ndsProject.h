#ifndef __NDSPROJECT_H__
#define __NDSPROJECT_H__

#include "Framework/framework.h"
#include "nds.h"

class NDSProject : public fw::Framework
{
private:
	touchPosition touchXY;
	int keys = 0;

protected:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
};

#endif
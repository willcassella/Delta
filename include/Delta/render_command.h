// render_command.h
#pragma once

#include "dt_common.h"

typedef struct dt_RenderCommand dt_RenderCommand;

struct dt_RenderCommand {
	float coords[6];
	dt_Color color;
};

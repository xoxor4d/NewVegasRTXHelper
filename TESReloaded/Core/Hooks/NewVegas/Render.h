#pragma once

extern bool render_skinned;
extern UINT ff_curr_bone_idx;

extern void pre_drawindexedprim();
extern void post_drawindexedprim();

void AttachRenderHooks();
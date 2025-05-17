#include "Hooks.h"
#include "Device.h"

typedef void (__fastcall* RenderListAdd)(void*, void*, void*, WORD, BYTE, BYTE, void*, void*, void*, void*);
static RenderListAdd RenderListOrig = (RenderListAdd)0xBA9EE0;

void __fastcall RenderListHook(void* ecx, void* edx, void* triStrips, WORD renderPassNum, BYTE enable, BYTE numLights, ShadowSceneLight* light0, ShadowSceneLight* light1, ShadowSceneLight* light2, ShadowSceneLight* light3) {
	// Block render passes to avoid confusing remix
	switch (renderPassNum) {
		// Normals
		case BSSM_DIFFUSEDIR_S:
		case BSSM_DIFFUSEPT2:
		case BSSM_DIFFUSEPT2_S:
		case BSSM_DIFFUSEPT2_SFg:
		case BSSM_DIFFUSEPT3:
		case BSSM_DIFFUSEPT3_S:
		case BSSM_DIFFUSEPT3_SFg:
		// Normals with bDoSpecularPass=1
		case BSSM_2x_SPECULARDIR:
		case BSSM_2x_SPECULARDIR_S:
		case BSSM_2x_SPECULARPT:
		case BSSM_2x_SPECULARPT_S:
		case BSSM_2x_SPECULARPT2:
		case BSSM_2x_SPECULARPT3:
		case BSSM_2x_SPECULARPT3_S:
		// Weird transparent black
		case BSSM_TEXTURE_Vc:
			return;
	}
	// Hide lights from game
	RenderListOrig(ecx, edx, triStrips, renderPassNum, enable, min(numLights, 1), light0, nullptr, nullptr, nullptr);
}


// < xo start

D3DXMATRIX IDENTITY =
{
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
};

IDirect3DVertexShader9* ff_og_shader = nullptr;
bool ff_was_modified = false;
bool ff_use_shader = false;

bool render_static = false;
bool render_skinned = false;
bool render_blended = false;

UINT ff_curr_bone_idx = 0u;

void pre_drawindexedprim()
{
	const auto& dev = TheRenderManager->device;
	//auto x = &TheRenderManager->PrePackObjects;
	bool force_world_transform = false;

	/*if (render_blended)
	{
		int x = 1;
	}*/

	if (render_skinned && ff_curr_bone_idx)
	{
		//if (TheSettingManager->SettingsMain.Remix.FixedFunctionLit)
		{
			//if (/*true || curr_bone_idx */)
			{
				dev->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_3WEIGHTS);
				dev->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
				//dev->SetTransform(D3DTS_WORLDMATRIX(i), (D3DMATRIX*)&mat);
			}
			/*else
			{
				force_world_transform = true;
			}*/
		}
	}
	else
	{
		dev->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
		dev->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	}


	if (render_static || render_skinned || render_blended)
	{
#if 0
		IDirect3DVertexDeclaration9* vertex_decl = nullptr;
		dev->GetVertexDeclaration(&vertex_decl);

		enum d3ddecltype : BYTE
		{
			D3DDECLTYPE_FLOAT1 = 0,		// 1D float expanded to (value, 0., 0., 1.)
			D3DDECLTYPE_FLOAT2 = 1,		// 2D float expanded to (value, value, 0., 1.)
			D3DDECLTYPE_FLOAT3 = 2,		// 3D float expanded to (value, value, value, 1.)
			D3DDECLTYPE_FLOAT4 = 3,		// 4D float
			D3DDECLTYPE_D3DCOLOR = 4,	// 4D packed unsigned bytes mapped to 0. to 1. range

			// Input is in D3DCOLOR format (ARGB) expanded to (R, G, B, A)
			D3DDECLTYPE_UBYTE4 = 5,		// 4D unsigned byte
			D3DDECLTYPE_SHORT2 = 6,		// 2D signed short expanded to (value, value, 0., 1.)
			D3DDECLTYPE_SHORT4 = 7,		// 4D signed short

			// The following types are valid only with vertex shaders >= 2.0
			D3DDECLTYPE_UBYTE4N = 8,	// Each of 4 bytes is normalized by dividing to 255.0
			D3DDECLTYPE_SHORT2N = 9,	// 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
			D3DDECLTYPE_SHORT4N = 10,	// 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
			D3DDECLTYPE_USHORT2N = 11,  // 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
			D3DDECLTYPE_USHORT4N = 12,  // 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
			D3DDECLTYPE_UDEC3 = 13,		// 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
			D3DDECLTYPE_DEC3N = 14,		// 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
			D3DDECLTYPE_FLOAT16_2 = 15,	// Two 16-bit floating point values, expanded to (value, value, 0, 1)
			D3DDECLTYPE_FLOAT16_4 = 16,	// Four 16-bit floating point values
			D3DDECLTYPE_UNUSED = 17,	// When the type field in a decl is unused.
		};
		enum d3ddecluse : BYTE
		{
			D3DDECLUSAGE_POSITION = 0,
			D3DDECLUSAGE_BLENDWEIGHT,   // 1
			D3DDECLUSAGE_BLENDINDICES,  // 2
			D3DDECLUSAGE_NORMAL,        // 3
			D3DDECLUSAGE_PSIZE,         // 4
			D3DDECLUSAGE_TEXCOORD,      // 5
			D3DDECLUSAGE_TANGENT,       // 6
			D3DDECLUSAGE_BINORMAL,      // 7
			D3DDECLUSAGE_TESSFACTOR,    // 8
			D3DDECLUSAGE_POSITIONT,     // 9
			D3DDECLUSAGE_COLOR,         // 10
			D3DDECLUSAGE_FOG,           // 11
			D3DDECLUSAGE_DEPTH,         // 12
			D3DDECLUSAGE_SAMPLE,        // 13
		};
		struct d3dvertelem
		{
			WORD Stream;		// Stream index
			WORD Offset;		// Offset in the stream in bytes
			d3ddecltype Type;	// Data type
			BYTE Method;		// Processing method
			d3ddecluse Usage;	// Semantics
			BYTE UsageIndex;	// Semantic index
		};

		d3dvertelem decl[MAX_FVF_DECL_SIZE]; UINT numElements = 0;
		vertex_decl->GetDeclaration((D3DVERTEXELEMENT9*)decl, &numElements);
		int x = 1;
#endif

		const auto ren = BSShaderManager::GetRenderer();
		//if (TheSettingManager->SettingsMain.Remix.FixedFunctionLit)
		{

			dev->GetVertexShader(&ff_og_shader);
			dev->SetVertexShader(nullptr);
			ff_was_modified = true;

			if (render_static || force_world_transform) {
				dev->SetTransform(D3DTS_WORLD, &ren->worldMatrix);
			}

			dev->SetTransform(D3DTS_VIEW, &ren->viewMatrix);
			dev->SetTransform(D3DTS_PROJECTION, &ren->projMatrix);
		}
		/*else
		{
			dev->SetTransform(D3DTS_WORLD, &IDENTITY);
		}*/
	}
}

void post_drawindexedprim()
{
	const auto& dev = TheRenderManager->device;
	if (ff_was_modified)
	{
		dev->SetVertexShader(ff_og_shader);
		dev->SetTransform(D3DTS_WORLD, &IDENTITY);
		dev->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
		dev->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	}

	//if (curr_bone_idx) {
		//curr_bone_idx = 0u;
	//}

	ff_was_modified = false;
	ff_use_shader = false;
}

__declspec(naked) void on_render_lit_surfs_stub()
{
	static uint32_t func_addr = 0xB98E80;
	static uint32_t retn_addr = 0xB995F5;
	__asm
	{
		mov		render_static, 1;

		mov     eax, [esp + 0x18];
		push    ecx;
		push    edx;
		push    eax;
		push    esi;
		call	func_addr;

		mov		render_static, 0;
		jmp		retn_addr;
	}
}

__declspec(naked) void on_render_skinned_stub()
{
	static uint32_t func_addr = 0xB99110;
	static uint32_t retn_addr = 0xB9959D;
	__asm
	{
		mov		render_skinned, 1;
		call	func_addr;
		mov		render_skinned, 0;
		mov 	ff_curr_bone_idx, 0;
		jmp		retn_addr;
	}
}

__declspec(naked) void reset_bones_stub()
{
	static uint32_t func_addr = 0x43D450;
	static uint32_t retn_addr = 0xB991EC;
	__asm
	{
		call	func_addr;
		mov 	ff_curr_bone_idx, 0;
		jmp		retn_addr;
	}
}

__declspec(naked) void on_blended_emissive_stub()
{
	static uint32_t func_addr = 0xB650C0;
	static uint32_t retn_addr = 0xB65C53;
	__asm
	{
		mov 	render_blended, 1;
		call	func_addr;
		mov 	render_blended, 0;
		jmp		retn_addr;
	}
}

// xo end >


void AttachRenderHooks()
{
	if (SettingManager::SkipRenderPass) {
		MH_CreateHookSimple((LPVOID*)&RenderListOrig, RenderListHook);
	}

	// xo - hook d3d device interface
	AttachDeviceHooks();

	// xo - render static meshes with fixed function
	if (SettingManager::UseFixedFunction) 
	{
		SafeWriteJump(0xB995E8, (UInt32)on_render_lit_surfs_stub);
		SafeWriteJump(0xB99598, (UInt32)on_render_skinned_stub);
		SafeWriteJump(0xB991E7, (UInt32)reset_bones_stub);
		SafeWrite8(0xB992F2, 0xEB); // fix broken skinning

		SafeWriteJump(0x8741DB, (UInt32)on_blended_emissive_stub);
	}
}

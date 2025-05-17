#pragma once

class NiNode;
class NiPick;
class NiTextKeyExtraData;
class NiFormArray;
class NiStringPalette;
class NiStream;
class NiTimeController;
class NiPropertyState;
class NiDynamicEffectState;
class NiProperty;
class NiFogProperty;
class NiAlphaProperty;
class NiMaterialProperty;
class NiTexturingProperty;
class NiCullingProcess;
class NiDynamicEffect;
class NiControllerManager;
class NiVisibleArray;
class NiAdditionalGeometryData;
class NiGeometryGroup;
class NiRenderedCubeMap;
class NiTexture;
class NiGeometry;
class NiTriBasedGeom;
class NiTriShape;
class NiTriStrips;
class NiPixelData;
class NiParticles;
class NiLines;
class NiGeometryGroupManager;
class NiScreenElements;
class NiSourceCubeMap;
class NiRenderer;
class Ni2DBuffer;
class NiD3DRenderStateGroup;
class NiD3DShaderProgram;
class NiD3DShader;
class NiDX9LightManager;
class NiDX9VertexBufferManager;
class NiDX9ShaderConstantManager;
class NiDX9IndexBufferManager;
class NiDX9TextureManager;
class NiDX9RenderedTextureData;
class NiDX9RenderedCubeMapData;
class NiDX9DynamicTextureData;
class NiDX9AdapterDesc;
class NiDX9DeviceDesc;
class NiDX9Renderer;
class NiDX9RenderState;
class NiDX92DBufferData;

class BSShaderAccumulator;

class ShadowSceneLight;
class AnimSequenceBase;

class TESObjectREFR;
class TESWaterCullingProcess;

class bhkCollisionObject;

#define CREATE_OBJECT(CLASS, ADDRESS) static CLASS* CreateObject() { return StdCall<CLASS*>(ADDRESS); };

struct NiRTTI {
	const char* name;
	NiRTTI*		parent;
};
static_assert(sizeof(NiRTTI) == 0x008);

#define NIRTTI_ADDRESS(address) \
	static inline const NiRTTI* const ms_RTTI = (NiRTTI*)address;

class NiPoint2 {
public:
	float	x;
	float	y;
};
static_assert(sizeof(NiPoint2) == 0x008);

class NiPoint3 {
public:
	float operator * (const NiPoint3 pt) const { return x * pt.x + y * pt.y + z * pt.z; }

	void GetLookAt(NiPoint3* LookAt, NiPoint3* Rotation);

	float x;
	float y;
	float z;

	D3DXVECTOR4		toD3DXVEC4() {
		return D3DXVECTOR4(x, y, z, 1.0);
	}
};
static_assert(sizeof(NiPoint3) == 0x00C);

class NiPoint4 {
public:
	float x;
	float y;
	float z;
	float w;
};
static_assert(sizeof(NiPoint4) == 0x10);

class NiVector4 {
public:
	void Normalize();

	float x;
	float y;
	float z;
	float w;
};
static_assert(sizeof(NiVector4) == 0x010);

class NiMatrix33 {
public:
	NiPoint3 operator * (const NiPoint3 pt) const {
		return {
			data[0][0] * pt.x + data[0][1] * pt.y + data[0][2] * pt.z,
			data[1][0] * pt.x + data[1][1] * pt.y + data[1][2] * pt.z,
			data[2][0] * pt.x + data[2][1] * pt.y + data[2][2] * pt.z
		};
	}
	NiMatrix33 operator * (const NiMatrix33 mat) const {
		NiMatrix33 prd;

		prd.data[0][0] =
			data[0][0] * mat.data[0][0]+
			data[0][1] * mat.data[1][0]+
			data[0][2] * mat.data[2][0];
		prd.data[1][0] =
			data[1][0] * mat.data[0][0]+
			data[1][1] * mat.data[1][0]+
			data[1][2] * mat.data[2][0];
		prd.data[2][0] =
			data[2][0] * mat.data[0][0]+
			data[2][1] * mat.data[1][0]+
			data[2][2] * mat.data[2][0];
		prd.data[0][1] =
			data[0][0] * mat.data[0][1]+
			data[0][1] * mat.data[1][1]+
			data[0][2] * mat.data[2][1];
		prd.data[1][1] =
			data[1][0] * mat.data[0][1]+
			data[1][1] * mat.data[1][1]+
			data[1][2] * mat.data[2][1];
		prd.data[2][1] =
			data[2][0] * mat.data[0][1]+
			data[2][1] * mat.data[1][1]+
			data[2][2] * mat.data[2][1];
		prd.data[0][2] =
			data[0][0] * mat.data[0][2]+
			data[0][1] * mat.data[1][2]+
			data[0][2] * mat.data[2][2];
		prd.data[1][2] =
			data[1][0] * mat.data[0][2]+
			data[1][1] * mat.data[1][2]+
			data[1][2] * mat.data[2][2];
		prd.data[2][2] =
			data[2][0] * mat.data[0][2]+
			data[2][1] * mat.data[1][2]+
			data[2][2] * mat.data[2][2];
		return prd;
	}
	void GenerateRotationMatrixZXY(NiPoint3* v, bool degrees);

	float data[3][3];
};
static_assert(sizeof(NiMatrix33) == 0x024);

class NiTransform {
public:
	NiMatrix33	rot;		// 00
	NiPoint3	pos;		// 24
	float		scale;		// 30
};
static_assert(sizeof(NiTransform) == 0x034);

class NiPlane {
public:
	enum {
		NoSide = 0,
		PositiveSide = 1,
		NegativeSide = 2,
	};

	NiPoint3	Normal;
	float		Constant;
};
static_assert(sizeof(NiPlane) == 0x010);

class NiBound {
public:
	UInt32 WhichSide(NiPlane* Plane);

	NiPoint3	Center;
	float		Radius;
};
static_assert(sizeof(NiBound) == 0x010);

class NiFrustum {
public:
	float	Left;		// 00
	float	Right;		// 04
	float	Top;		// 08
	float	Bottom;		// 0C
	float	Near;		// 10
	float	Far;		// 14
	UInt8	Ortho;		// 18
	UInt8	pad18[3];
};
static_assert(sizeof(NiFrustum) == 0x01C);

class NiFrustumPlanes {
public:
	enum {
		NearPlane = 0,
		FarPlane = 1,
		LeftPlane = 2,
		RightPlane = 3,
		TopPlane = 4,
		BottomPlane = 5,
		MaxPlanes = 6
	};

	NiPlane	CullingPlanes[MaxPlanes];	// 00
	UInt32	ActivePlanes;				// 60
};
static_assert(sizeof(NiFrustumPlanes) == 0x064);

class NiViewport {
public:
	float	l;
	float	r;
	float	t;
	float	b;
};
static_assert(sizeof(NiViewport) == 0x010);

template <class T> class NiRect {
public:
	NiRect(T left = T(0), T right = T(0), T top = T(0), T bottom = T(0))
	{
		m_left = left;
		m_right = right;
		m_top = top;
		m_bottom = bottom;
	}

	T m_left, m_right, m_top, m_bottom;
};
static_assert(sizeof(NiRect<float>) == 0x10);

class NiColor {
public:
	float	r;
	float	g;
	float	b;
};
static_assert(sizeof(NiColor) == 0x00C);

class NiColorAlpha {
public:
	float	r;
	float	g;
	float	b;
	float	a;
};
static_assert(sizeof(NiColorAlpha) == 0x010);

template <typename T>
class NiTList {
public:
	struct Entry {
		Entry* next;
		Entry* prev;
		T* data;
	};

	Entry*	start;		// 000
	Entry*	end;		// 004
	UInt32	numItems;	// 008
};
static_assert(sizeof(NiTList<void>) == 0x00C);

template <typename TKey, typename TData>
class NiTMap {
public:
	struct Entry {
		Entry*	next;
		TKey	key;
		TData*	data;
	};

	virtual UInt32	CalculateBucket(UInt32 key);
	virtual bool	CompareKey(UInt32 lhs, UInt32 rhs);
	virtual void	Fn_03(UInt32 arg0, UInt32 arg1, UInt32 arg2);	// assign to entry
	virtual void	Fn_04(UInt32 arg);
	virtual void	Fn_05();	// locked operations
	virtual void	Fn_06();	// locked operations

	UInt32			m_numBuckets;	// 4
	Entry**			m_buckets;		// 8
	UInt32			m_numItems;		// C
};
static_assert(sizeof(NiTMap<void, void>) == 0x010);

template <typename T>
class NiTArray {
public:
	UInt16			Add(T* Item);

	void**	_vtbl;			// 00
	T*		data;			// 04
	UInt16	capacity;		// 08 - init'd to size of preallocation
	UInt16	end;			// 0A - index of the first free entry in the block of free entries at the end of the array (or numObjs if full)
	UInt16	numObjs;		// 0C - init'd to 0
	UInt16	growSize;		// 0E - init'd to size of preallocation
};
static_assert(sizeof(NiTArray<void>) == 0x010);

class NiPixelFormat {
public:
	enum Format {
		kFormat_RGB = 0,
		kFormat_RGBA,
		kFormat_PAL,
		kFormat_PALALPHA,
		kFormat_DXT1,
		kFormat_DXT3,
		kFormat_DXT5,
		kFormat_RGB24NONINTERLEAVED,
		kFormat_BUMP,
		kFormat_BUMPLUMA,
		kFormat_RENDERERSPECIFIC,
		kFormat_ONE_CHANNEL,
		kFormat_TWO_CHANNEL,
		kFormat_THREE_CHANNEL,
		kFormat_FOUR_CHANNEL,
		kFormat_DEPTH_STENCIL,
		kFormat_UNKNOWN,
		kFormat_MAX
	};

	enum Component {
		kComp_RED = 0,
		kComp_GREEN,
		kComp_BLUE,
		kComp_ALPHA,
		kComp_COMPRESSED,
		kComp_OFFSET_U,
		kComp_OFFSET_V,
		kComp_OFFSET_W,
		kComp_OFFSET_Q,
		kComp_LUMA,
		kComp_HEIGHT,
		kComp_VECTOR_X,
		kComp_VECTOR_Y,
		kComp_VECTOR_Z,
		kComp_PADDING,
		kComp_INTENSITY,
		kComp_INDEX,
		kComp_DEPTH,
		kComp_STENCIL,
		kComp_EMPTY,
		kComp_MAX
	};

	enum Representation {
		kRep_NORM_INT = 0,
		kRep_HALF,
		kRep_FLOAT,
		kRep_INDEX,
		kRep_COMPRESSED,
		kRep_UNKNOWN,
		kRep_INT,
		kRep_MAX
	};

	enum Tiling {
		kTile_NONE = 0,
		kTile_XENON,
		kTile_MAX
	};

	struct NiComponentSpec {
		Component		eComponent;			// 0
		Representation	eRepresentation;	// 4
		UInt8			BitsPerComponent;	// 8
		UInt8			Signed;				// 9
		UInt8			padA[2];			// A
	};

	UInt8			BitsPerPixel;	// 00
	UInt8			SRGBSpace;		// 01
	UInt8			pad02[2];
	Format			eFormat;		// 04
	Tiling			eTiling;		// 08
	UInt32			RendererHint;	// 0C
	UInt32			ExtraData;		// 10
	NiComponentSpec	Components[4];	// 14
};
static_assert(sizeof(NiPixelFormat) == 0x044);

class NiRefObject {
public:
	virtual void		Destructor(bool freeThis);	// 00
	virtual void		Free(void);					// 01

	UInt32				m_uiRefCount;	// 004
};
static_assert(sizeof(NiRefObject) == 0x008);

class NiObject : public NiRefObject {
public:
	virtual NiRTTI*		GetType();		// 02
	virtual NiNode*		GetAsNiNode();	// 03 
	virtual UInt32		Unk_04();		// 04
	virtual UInt32		Unk_05();		// 05
	virtual UInt32		Unk_06();		// 06
	virtual UInt32		Unk_07();		// 07
	virtual UInt32		Unk_08();		// 08
	virtual UInt32		Unk_09();		// 09
	virtual UInt32		Unk_0A();		// 0A
	virtual UInt32		Unk_0B();		// 0B
	virtual UInt32		Unk_0C();		// 0C
	virtual UInt32		Unk_0D();		// 0D
	virtual UInt32		Unk_0E();		// 0E
	virtual UInt32		Unk_0F();		// 0F
	virtual UInt32		Unk_10();		// 10
	virtual UInt32		Unk_11();		// 11
	virtual NiObject*	Copy();			// 12
	virtual void		Load(NiStream* stream);
	virtual void		PostLoad(NiStream* stream);
	virtual void		FindNodes(NiStream* stream);
	virtual void		Save(NiStream* stream);
	virtual bool		Compare(NiObject* obj);
	virtual void		DumpAttributes(NiTArray <char*>* dst);
	virtual void		DumpChildAttributes(NiTArray <char*>* dst);
	virtual void		Unk_1A();
	virtual void		Unk_1B(UInt32 arg);
	virtual void		Unk_1C();
	virtual void		GetType2();	
	virtual void		Unk_1E(UInt32 arg);
	virtual void		Unk_1F();
	virtual void		Unk_20();
	virtual void		Unk_21();
	virtual void		Unk_22();
    
    
    void LogObjectAttributes();
};
static_assert(sizeof(NiObject) == 0x008);

class NiExtraData : public NiObject {
public:
	virtual	bool	Unk_23();
	virtual bool	Unk_24();

	char*	m_pcName;	// 08
};
static_assert(sizeof(NiExtraData) == 0x00C);

class NiObjectNET : public NiObject {
public:
	void				SetName(const char* Name) { char* (__cdecl* CreateString)(const char*) = (char* (__cdecl*)(const char*))0x00A5B690; char* S = CreateString(Name); ThisCall(0x00A5B950, this, &S); }

	const char*			m_pcName;						// 008
	NiTimeController*	m_controller;					// 00C
	NiExtraData**		m_extraDataList;				// 010
	UInt16				m_extraDataListLen;				// 014
	UInt16				m_extraDataListCapacity;		// 016
};
static_assert(sizeof(NiObjectNET) == 0x018);

struct PropertyNode{
    PropertyNode* next;
    PropertyNode* prev;
    NiProperty* data;
};
struct PropertyList {
    PropertyNode* first;
    PropertyNode* last;
    UInt32 count;
};


template <std::size_t N> struct Debug; // No definition

template <> struct Debug<12> {}; // Definition for "old" sizeof;

template struct Debug<sizeof(PropertyNode)>; // Issue error if definition is missing

class NiAVObject : public NiObjectNET {
public:
	virtual void			UpdateControllers(float fTime);
	virtual void			ApplyTransform(void);
	virtual void			Unk_25();
	virtual NiAVObject*		GetObjectByName(const char* Name);
	virtual void*			Unk_27();
	virtual void			UpdateDownwardPass(float fTime, bool bUpdateControllers);
	virtual void			UpdateSelectedDownwardPass(float fTime);
	virtual void			UpdateRigidDownwardPass(float fTime);
	virtual void			UpdatePropertiesDownward(NiPropertyState* ParentState);
	virtual void			UpdateEffectsDownward(NiDynamicEffectState* ParentState);
	virtual void			UpdateTransformData();
	virtual void			UpdateTransformBound();
	virtual void			Unk_2F(NiCullingProcess* tgt);
	virtual void			Unk_30(void* arg);			// get NiMaterialProperty, pass to arg if found
	virtual void			Unk_31(void* arg);
	virtual void			Unk_32(void* arg);
	virtual void			DetachParent(void* arg);
	virtual void			Unk_34(void* arg);
	virtual void			OnVisible(NiCullingProcess* kCuller);
	virtual void			PurgeRendererData(void* arg);	// last is 036 verified

	enum {
		kFlag_AppCulled					= 1 << 0,
		kFlag_SelUpdate					= 1 << 1,
		kFlag_SelUpdateTransforms		= 1 << 2,
		kFlag_SelUpdatePropControllers	= 1 << 3,
		kFlag_SelUpdateRigid			= 1 << 4,
	};

	float GetDistance(NiPoint3* Point);
	NiBound*	GetWorldBound();
	float		GetWorldBoundRadius();
    
	NiNode*					m_parent;				// 018
	UInt32					unk001C;				// 01C
	NiBound*				m_kWorldBound;			// 020
	PropertyList			propertyList;			// 024
	UInt32					m_flags;				// 030
	NiTransform				m_localTransform;		// 034
	NiTransform				m_worldTransform;		// 068
};
static_assert(sizeof(NiAVObject) == 0x9C);

class NiNode : public NiAVObject {
public:
	virtual void		AddObject(NiAVObject* nChild, UInt8 FirstAvail); // Add child node
	virtual NiAVObject* RemoveObject(NiAVObject** RemovedChild, NiAVObject* nChild);
	virtual NiAVObject* RemoveObjectAt(NiAVObject** RemovedChild, UInt32 Index);
	virtual NiAVObject* SetObjectAt(NiAVObject** SetChild, UInt32 Index, NiAVObject* nChild);
	virtual void		ApplyTransform(const NiMatrix33& Mat, const NiPoint3& Trn, bool OnLeft);
	virtual bool		Unk_3B();
	virtual bool		Unk_3C();
	virtual bool		Unk_3D();
	virtual bool		Unk_3E();
	virtual bool		Unk_3F();
	
	void				New(UInt16 Children);

	NiTArray<NiAVObject*>	m_children;	// 09C
};
static_assert(sizeof(NiNode) == 0xAC);

class NiBillboardNode : public NiNode {
public:
	virtual void	Unk_40();

	UInt32			unkAC[2];		// AC
};
static_assert(sizeof(NiBillboardNode) == 0xB4);

class NiCamera : public NiAVObject {
public:
	D3DMATRIX 	    WorldToCam;	        // 94
	NiFrustum		Frustum;			// D4
	float			MinNearPlaneDist;	// F8
	float			MaxFarNearRatio;	// FC
	NiViewport		ViewPort;			// 100
	float			LODAdjust;			// 110
};
static_assert(sizeof(NiCamera) == 0x114);

class SceneGraph : public NiNode {
public:
	void				UpdateParticleShaderFoV(float FoV);
	void				SetNearDistance(float Distance);
    
	NiCamera*			camera;					// AC
	UInt32				unkB0;					// B0
	NiCullingProcess*	cullingProcess;			// B4
	UInt8				IsMinFarPlaneDistance;	// B8 The farplane is set to 20480.0 when the flag is true. Probably used for interiors.
	UInt8				padB8[3];
	float				cameraFOV;				// BC
};
static_assert(sizeof(SceneGraph) == 0xC0);

class NiDynamicEffect : public NiAVObject {
public:
	UInt8			On;				// 9C
	UInt8			unk9D;			// 9D
	UInt8			CastShadows;	// 9E
	UInt8			CanCarry;		// 9F
	UInt32			Index;			// A0
	UInt32			unkA4;			// A4
	UInt32			unkA8;			// A8
	UInt32			unkAC;			// AC
	UInt32			unkB0;			// B0
	UInt32			unkB4;			// B4
	UInt32			unkB8;			// B8
	UInt32			unkBC;			// BC
	UInt32			unkC0;			// C0
};
static_assert(sizeof(NiDynamicEffect) == 0xC4);

class NiLight : public NiDynamicEffect {
public:
	float			Dimmer;	// C4
	NiColor			Amb;	// C8
	NiColor			Diff;	// D4
	NiColor			Spec;	// E0
	void*			unk104;	// EC
};
static_assert(sizeof(NiLight) == 0xF0);

class NiPointLight : public NiLight {
public:
	float			Atten0;		// F0
	float			Atten1;		// F4
	float			Atten2;		// F8
};
static_assert(sizeof(NiPointLight) == 0xFC);

class NiDirectionalLight : public NiLight {
public:
	NiPoint3		direction;			// F0
};
static_assert(sizeof(NiDirectionalLight) == 0xFC);

class NiVBChip {
public:
	UInt32					Index;      // 00
	UInt32					Unk04;		// 04 union { NiVBBlock* m_pkBlock; NiVBDynamicSet* m_pkDynamicSet; };
	IDirect3DVertexBuffer9* VB;			// 08
	UInt32					Offset;		// 0C
	UInt32					LockFlags;	// 10
	UInt32					Size;		// 14
	NiVBChip*				Next;		// 18
	NiVBChip*				Prev;		// 1C
};
static_assert(sizeof(NiVBChip) == 0x020);

class NiGeometryBufferData {
public:
	virtual void		Destructor(bool freeThis);			// 00
	virtual void		Free();								// 01

	UInt32							Flags;					// 04
	NiGeometryGroup*				GeometryGroup;			// 08
	UInt32							FVF;					// 0C
	IDirect3DVertexDeclaration9*	VertexDeclaration;		// 10
	UInt32							unk10;					// 14
	UInt32							VertCount;				// 18
	UInt32							MaxVertCount;			// 1C
	UInt32							StreamCount;			// 20
	UInt32*							VertexStride;			// 24
	NiVBChip**						VBChip;					// 28
	UInt32							IndexCount;				// 2C
	UInt32							IBSize;					// 30
	IDirect3DIndexBuffer9*			IB;						// 34
	UInt32							BaseVertexIndex;		// 38
	D3DPRIMITIVETYPE				PrimitiveType;			// 3C
	UInt32							TriCount;				// 40
	UInt32							MaxTriCount;			// 44
	UInt32							NumArrays;				// 48
	UInt16*							ArrayLengths;			// 4C
	UInt16*							IndexArray;				// 50
};
static_assert(sizeof(NiGeometryBufferData) == 0x54);

class NiGeometryData : public NiObject {
public:
	enum Consistency {
		MUTABLE			 = 0x0000,
		STATIC			 = 0x4000,
		VOLATILE		 = 0x8000,
		CONSISTENCY_MASK = 0xF000,
	};

	UInt16						Vertices;			// 008
	UInt16						Unk00A;				// 00A
	UInt16						Unk00C;				// 00C
	UInt16						Unk00E;				// 00E
	UInt32						Unk010;				// 010
	UInt32						Unk014;				// 014
	UInt32						Unk018;				// 018
	UInt32						Unk01C;				// 01C
	NiPoint3*					Vertex;				// 020
	NiPoint3*					Normal;				// 024
	NiColorAlpha*				Color;				// 028
	UInt32						Texture;			// 02C NiPoint2*
	UInt32						Unk030;				// 030
	NiGeometryBufferData*		BuffData;			// 034
	UInt8						Unk038;				// 038
	UInt8						Unk039;				// 039
	UInt8						Unk03A;				// 03A
	UInt8						Unk03B;				// 03B
	UInt8						Unk03C;				// 03C
	UInt8						pad03C[3];
};
static_assert(sizeof(NiGeometryData) == 0x40);

class NiSkinPartition : public NiObject {
public:
	class Partition {
	public:
		void**					_vtbl;			// 00
		UInt16*					pBones;			// 04
		float*					Weights;		// 08
		UInt32					Unk04[4];		// 0C
		UInt16					Vertices;		// 1C
		UInt16					Triangles;		// 1E
		UInt16					Bones;			// 20
		UInt16					Strips;			// 22
		UInt16					BonesPerVertex; // 24
		UInt16					pad28;
		NiGeometryBufferData*	BuffData;		// 28
	};

	UInt32		PartitionsCount;		// 08
	Partition*	Partitions;				// 0C
};
static_assert(sizeof(NiSkinPartition) == 0x10);

class NiSkinData : public NiObject {
public:
	class BoneVertData {
	public:
		UInt16	Verts;		// 00
		float	Weight;		// 04
	};

	class BoneData {
	public:
		NiTransform		SkinToBone;		// 00
		NiBound			Bound;			// 34
		BoneVertData*	BoneVertData;	// 44
		UInt16			Verts;			// 48
		UInt16			pad44;
	};

	NiSkinPartition*	SkinPartition;		// 08
	NiTransform			RootParentToSkin;	// 0C
	BoneData*			BoneData;			// 40
	UInt32				Bones;				// 44
};
static_assert(sizeof(NiSkinData) == 0x48);

class NiSkinInstance : public NiObject {
public:
	NiSkinData*			SkinData;				// 08
	NiSkinPartition*	SkinPartition;			// 0C
	NiAVObject*			RootParent;				// 10
	NiAVObject**		BoneObjects;			// 14
	UInt32				FrameID;				// 18
	UInt32				Bones;					// 1C
	UInt32				BoneRegisters;			// 20
	UInt32				BoneSize;				// 24
	void*				BoneMatrixes;			// 28 Matrixes array for the bones D3DMATRIX
	void*				SkinToWorldWorldToSkin;	// 2C D3DMATRIX
	UInt32				Unk30;					// 30
	
	bool IsPartitionEnabled(UInt32 partitionIndex);
    
};
static_assert(sizeof(NiSkinInstance) == 0x34);

class DismemberPartition {
public:
    UInt8 Enabled;
    UInt8 StartCap;  //Questionable, but it's the only way I can make sense of that code
    UInt16 bodyPart;
}; 
static_assert(sizeof(DismemberPartition) == 4);

class BSDismemberSkinInstance : public NiSkinInstance {
public:
    UInt32 partitionNumber;
    DismemberPartition* partitions;
    UInt8  IsRenderable;  //In Load this is made in OR with every partition->Enabled flag
    UInt8  pad[3];
};
static_assert(sizeof(BSDismemberSkinInstance) == 0x40);

class NiProperty : public NiObjectNET {
public:
	enum PropertyType {
		kType_Alpha,
		kType_Culling,
		kType_Material,
		kType_Shade,
		kType_Stencil,
		kType_Texturing,
	};
    virtual PropertyType GetPropertyType();
};
static_assert(sizeof(NiProperty) == 0x18);

class NiShadeProperty : public NiProperty {
public:
	enum ShaderPropType : UInt32
	{
		kProp_Lighting = 0x1,
		kProp_DistantLOD = 0x2,
		kProp_GeometryDecal = 0x3,
		kProp_TallGrass = 0x4,
		kProp_SpeedTreeLeaf = 0x6,
		kProp_PPLighting = 0x8,
		kProp_Hair = 0x9,
		kProp_SpeedTreeBranch = 0xA,
		kProp_SpeedTreeBillboard = 0xB,
		kProp_Lighting30 = 0xC,
		kProp_Sky = 0xD,
		kProp_Water = 0xE,
		kProp_Bolt = 0xF,
		kProp_Particle = 0x11,
		kProp_Precipitation = 0x12,
		kProp_Tile = 0x13,
		kProp_NoLighting = 0x15,
		kProp_VolumetricFog = 0x16,
		kProp_BloodSplatter = 0x17,
		kProp_DistantTree = 0x18,
	};

	enum ExtraFlags {
		kSmooth = 1 << 0, // always set by default, doesn't do anything
		kFirstPerson = 1 << 1, // marks player's view model geometry
		kThirdPerson = 1 << 2, // marks player's world model geometry
	};

	Bitfield16		m_usFlags;
	ShaderPropType	m_eShaderType;

	NIRTTI_ADDRESS(0x11F5AE0);
};
static_assert(sizeof(NiShadeProperty) == 0x20);

class NiStencilProperty : public NiProperty {
public:
	enum TestFunc {
		TEST_NEVER,
		TEST_LESS,
		TEST_EQUAL,
		TEST_LESSEQUAL,
		TEST_GREATER,
		TEST_NOTEQUAL,
		TEST_GREATEREQUAL,
		TEST_ALWAYS,
		TEST_MAX
	};

	enum Action {
		ACTION_KEEP,
		ACTION_ZERO,
		ACTION_REPLACE,
		ACTION_INCREMENT,
		ACTION_DECREMENT,
		ACTION_INVERT,
		ACTION_MAX
	};

	enum {
		ENABLE_MASK = 0x1,
		FAILACTION_MASK = 0xE,
		FAILACTION_POS = 0x1,
		ZFAILACTION_MASK = 0x70,
		ZFAILACTION_POS = 0x4,
		PASSACTION_MASK = 0x380,
		PASSACTION_POS = 0x7,
		DRAWMODE_MASK = 0xC00,
		DRAWMODE_POS = 0xA,
		TESTFUNC_MASK = 0xF000,
		TESTFUNC_POS = 0xC,
	};

	enum DrawMode {
		DRAW_CCW_OR_BOTH = 0,
		DRAW_CCW = 1,
		DRAW_CW = 2,
		DRAW_BOTH = 3,
		DRAW_MAX,
	};

	Bitfield16	m_usFlags;
	UInt32		m_uiRef;
	UInt32		m_uiMask;

	CREATE_OBJECT(NiStencilProperty, 0xA6F410);

	bool IsEnabled() const {
		return m_usFlags.GetBit(ENABLE_MASK);
	}

	void SetDrawMode(NiStencilProperty::DrawMode aeDraw) {
		m_usFlags.SetField(aeDraw, DRAWMODE_MASK, DRAWMODE_POS);
	}

	NiStencilProperty::DrawMode GetDrawMode() const {
		return (NiStencilProperty::DrawMode)m_usFlags.GetField(DRAWMODE_MASK, DRAWMODE_POS);
	}
};
static_assert(sizeof(NiStencilProperty) == 0x24);

class NiPropertyState {
public:
	enum PropertyID {
		ALPHA = 0,
		CULLING = 1,
		MATERIAL = 2,
		SHADE = 3,
		STENCIL = 4,
		TEXTURING = 5,
		UNK = 6,
		MAX,
	};

	union {
		struct {
			NiAlphaProperty* m_spAlphaProperty;
			NiProperty* m_spCullingProperty;
			NiMaterialProperty* m_spMaterialProperty;
			NiShadeProperty* m_spShadeProperty;
			NiStencilProperty* m_spStencilProperty;
			NiTexturingProperty* m_spTextureProperty;
			NiProperty* m_spUnknownProperty;
		};
		NiProperty* m_aspProps[MAX];
	};

	template <class T>
	T* GetShadeProperty() const { return static_cast<T*>(m_spShadeProperty); };
};
static_assert(sizeof(NiPropertyState) == 0x1C);

class NiGeometry : public NiAVObject {
public:

	virtual void	RenderImmediate(NiRenderer* pkRenderer);
	virtual void	RenderImmediateAlt(NiRenderer* pkRenderer);
	virtual void	SetModelData(NiGeometryData* pkModelData);
	virtual void	CalculateNormals();
	virtual void	CalculateConsistency(bool bTool);

	NiProperty* GetProperty(NiProperty::PropertyType Type);

	NiPropertyState		propertyState;	// 9C
	NiGeometryData* geomData;		// B8
	NiSkinInstance* skinInstance;	// BC This seems to be a BSDismemberSkinInstance (old NiSkinInstance constructor is never used)
	NiD3DShader* shader;			// C0
};
static_assert(sizeof(NiGeometry) == 0xC4);

class NiDX9TextureData : public NiObject {
public:
	NiTexture*				parent;			// 08
	UInt32					width;			// 0C
	UInt32					height;			// 10
	NiPixelFormat			PixelFormat;	// 14
	UInt8					Unk58;			// 58
	UInt8					Unk59;			// 59
	UInt8					Unk5A;			// 5A
	UInt8					Unk5B;			// 5B
	UInt32					Unk5C;			// 5C
	NiDX9Renderer*			pRenderer;		// 60
	IDirect3DBaseTexture9*	dTexture;		// 64
	UInt32					Levels;			// 68
};
static_assert(sizeof(NiDX9TextureData) == 0x6C);

class NiDX9SourceTextureData : public NiDX9TextureData {
public:
	UInt8					ReplacementData;	// 6C
	UInt8					Mipmap;				// 6D
	UInt8					pad64[2];
	UInt32					FormattedSize;		// 70
	UInt32					Palette;			// 74 NiPalette
	UInt32					LevelsSkipped;		// 78
	UInt32					SourceRevID;		// 7C
	UInt32					PalRevID;			// 80
};
static_assert(sizeof(NiDX9SourceTextureData) == 0x84);

class NiTexture : public NiObjectNET {
public:
	virtual UInt32	GetWidth();
	virtual UInt32	GetHeight();
	virtual void	Unk_25();
	virtual void	Unk_26();
	virtual void	Unk_27();
	virtual void	Unk_28();

	enum PixelLayout {
		kPixelLayout_Palettized8 = 0,
		kPixelLayout_HighColor16,
		kPixelLayout_TrueColor32,
		kPixelLayout_Compressed,
		kPixelLayout_Bumpmap,
		kPixelLayout_Palettized4,
		kPixelLayout_PixDefault,
		kPixelLayout_SingleColor8,
		kPixelLayout_SingleColor16,
		kPixelLayout_SingleColor32,
		kPixelLayout_DoubleColor32,
		kPixelLayout_DoubleColor64,
		kPixelLayout_FloatColor32,
		kPixelLayout_FloatColor64,
		kPixelLayout_FloatColor128,
		kPixelLayout_SingleColor4,
	};

	enum AlphaFormat {
		kAlpha_None = 0,
		kAlpha_Binary,	// 1bpp
		kAlpha_Smooth,	// 8bpp
		kAlpha_Default,
	};

	enum MipMapFlag {
		kMipMap_Disabled = 0,
		kMipMap_Enabled,
		kMipMap_Default,
	};

	struct FormatPrefs {
		PixelLayout pixelLayout;
		AlphaFormat alphaFormat;
		MipMapFlag	mipmapFormat;
	};

	FormatPrefs			formatPrefs;	// 018
	NiDX9TextureData*	rendererData;	// 024
	NiTexture*			nextTex;		// 028 - linked list updated in ctor/dtor
	NiTexture*			prevTex;		// 02C
};
static_assert(sizeof(NiTexture) == 0x30);

class NiSourceTexture : public NiTexture {
public:
	virtual void	Unk_29();
	virtual void	Unk_2A();
	virtual void	Unk_2B();

	char*			ddsPath1;		// 30
	char*			ddsPath2;		// 34
	UInt32			unk38;			// 38
	UInt32			unk3C;			// 3C
	UInt32			unk40;			// 40
	UInt32			unk44;			// 44
};
static_assert(sizeof(NiSourceTexture) == 0x48);

class NiRenderedTexture : public NiTexture {
public:
	virtual Ni2DBuffer* GetBuffer();

	Ni2DBuffer* buffer;		// 030
	UInt32		unk034;		// 034
	UInt32		unk038;		// 038
	UInt32		unk03C;		// 03C
};
static_assert(sizeof(NiRenderedTexture) == 0x040);

class NiD3DTextureStage;
class NiD3DShaderConstantMap;

enum ShaderDefinitionEnum
{
  kShaderDefinition_ShadowLightShader = 0x1,
  kShaderDefinition_TallGrassShader = 0x2,
  kShaderDefinition_DistantLODShader = 0x3,
  kShaderDefinition_SpeedTreeBranchShader = 0x4,
  kShaderDefinition_SpeedTreeFrondShader = 0x5,
  kShaderDefinition_SpeedTreeLeafShader = 0x6,
  kShaderDefinition_BSShaderBloodSplatter = 0x7,
  kShaderDefinition_BSDistantTreeShader = 0x8,
  kShaderDefinition_NiD3DDefaultShader = 0x9,
  kShaderDefinition_SkyShader = 0xA,
  kShaderDefinition_HairShader = 0xD,
  kShaderDefinition_SkinShader = 0xE,
  kShaderDefinition_ParallaxShader = 0xF,
  kShaderDefinition_NiDX9ShaderDeclaration = 0x10,
  kShaderDefinition_WaterShader = 0x11,
  kShaderDefinition_ParticleShader = 0x18,
  kShaderDefinition_BoltShader = 0x19,
  kShaderDefinition_BeamShader = 0x1A,
  kShaderDefinition_Lighting30Shader = 0x1D,
  kShaderDefinition_PrecipitationShader = 0x1E,
  kShaderDefinition_TileShader = 0x20,
  kShaderDefinition_BSShaderNoLighting = 0x21,
  kShaderDefinition_VolumetricFogShader = 0x22,
};


class NiD3DShaderDeclaration : public NiObject {
public:
	NiDX9Renderer*				Renderer;		// 008
	NiDX9VertexBufferManager*	BufferManager;	// 00C
	IDirect3DDevice9*			Device;			// 010
	UInt32						Unk014;			// 014
	UInt32						Unk018;			// 018
	UInt32						Unk01C;			// 01C
	UInt32						StreamCount;	// 020
	UInt32						Unk024;			// 024
	UInt8						Unk028;			// 028
	UInt8						Unk029[3];		// 029

};
static_assert(sizeof(NiD3DShaderDeclaration) == 0x02C);

class NiDX9ShaderDeclaration : public NiD3DShaderDeclaration {
public:
	UInt32		Unk02C;			// 02C
	UInt32		Unk030;			// 030
	UInt8		Unk034;			// 034
	UInt8		Unk035[3];		// 035

};
static_assert(sizeof(NiDX9ShaderDeclaration) == 0x038);

class NiD3DShaderProgram : public NiRefObject {
public:
	enum ProgramType {
		PROGRAM_VERTEX = 0,
		PROGRAM_PIXEL,
		PROGRAM_GEOMETRY,
		PROGRAM_MAX
	};

	ProgramType			eProgramType;		// 08
	char*				Name;				// 0C
	char*				ShaderProgramName;	// 10
	UInt32				CodeSize;			// 14
	void*				Unk018;				// 18
	void*				Unk01C;				// 1C
	IDirect3DDevice9*	Device;				// 20
	NiDX9Renderer*		Renderer;			// 24
	NiDX9RenderState*	RenderState;		// 28
};
static_assert(sizeof(NiD3DShaderProgram) == 0x2C);

class NiD3DVertexShader : public NiD3DShaderProgram {
public:
	UInt8							SoftwareVP;		// 2C
	UInt8							pad[3];			// 2D
	UInt32							Usage;			// 30
	IDirect3DVertexShader9*			ShaderHandle;	// 34
	IDirect3DVertexDeclaration9*	Declaration;	// 38
};
static_assert(sizeof(NiD3DVertexShader) == 0x3C);

class NiD3DPixelShader : public NiD3DShaderProgram {
public:
	IDirect3DPixelShader9* ShaderHandle;	// 2C
};
static_assert(sizeof(NiD3DPixelShader) == 0x30);

class NiD3DPass {
public:
	void**							_vtbl;						// 00
	char							Name[16];					// 04
	UInt32							CurrentStage;				// 14
	UInt32							StageCount;					// 18
	UInt32							TexturesPerPass;			// 1C
	NiTArray<NiD3DTextureStage*>	Stages;						// 20
	NiD3DRenderStateGroup*			RenderStateGroup;			// 30
	NiD3DShaderConstantMap*			PixelConstantMap;			// 34
	char*							PixelShaderProgramFile;		// 38
	char*							PixelShaderEntryPoint;		// 3C
	char*							PixelShaderTarget;			// 40
	NiD3DPixelShader*				PixelShader;				// 44
	NiD3DShaderConstantMap*			VertexConstantMap;			// 48
	UInt8							Unk04C;						// 4C
	UInt8							pad04C[3];
	char*							VertexShaderProgramFile;	// 50
	char*							VertexShaderEntryPoint;		// 54
	char*							VertexShaderTarget;			// 58
	NiD3DVertexShader*				VertexShader;				// 5C
	UInt8							SoftwareVP;					// 60
	UInt8							RendererOwned;				// 61
	UInt8							pad[2];
	UInt32							RefCount;					// 64
};
static_assert(sizeof(NiD3DPass) == 0x68);

class NiShader : public NiRefObject {
public:
	char*		Name;					// 008
	UInt32		Unk00C;					// 00C
	UInt8		Unk010;					// 010
	UInt8		pad010[3];
};
static_assert(sizeof(NiShader) == 0x14);

class NiD3DShaderInterface : public NiShader {
public:
	IDirect3DDevice9*	D3DDevice;		// 014
	NiDX9Renderer*		D3DRenderer;	// 018
	NiDX9RenderState*	D3DRenderState;	// 01C
	UInt8				Unk020;			// 020
	UInt8				pad01C[3];
};
static_assert(sizeof(NiD3DShaderInterface) == 0x24);

class NiD3DShader : public NiD3DShaderInterface {
public:
	UInt8					IsInitialized;		// 024
	UInt8					Unk021;				// 025
	UInt8					pad021[2];
	NiD3DShaderDeclaration* ShaderDeclaration;	// 028
	NiD3DRenderStateGroup*  RenderStateGroup;	// 02C
	NiD3DShaderConstantMap* PixelConstantMap;	// 030
	NiD3DShaderConstantMap* VertexConstantMap;	// 034
	UInt32					Unk038[14];			// 038
};
static_assert(sizeof(NiD3DShader) == 0x70);

class BSShader : public NiD3DShader {
public:
	UInt32		Unk070;			// 070
	UInt32		Unk074;			// 074
	UInt32		Unk078;			// 078
};
static_assert(sizeof(BSShader) == 0x7C);

class WaterShader : public BSShader {
public:
	UInt32				Unk07C[70];		// 07C
	NiD3DVertexShader*	Vertex[3];		// 194
	NiD3DPixelShader*	Pixel[38];		// 1A0
	UInt32				Unk238[6];		// 238
};
static_assert(sizeof(WaterShader) == 0x250);


class ShadowLightShader : public BSShader{
public:
	UInt32 unk07C[4];
};
static_assert(sizeof(ShadowLightShader) == 0x8C);


class ParallaxShader : public ShadowLightShader{
public:
	NiD3DVertexShader*  Vertex[20];
 	NiD3DPixelShader*   Pixel[33];
};
static_assert(sizeof(ParallaxShader) == 0x160);

class BSImageSpaceShader : public BSShader {
public:
	UInt32				Unk07C[18];	// 07C
	NiD3DVertexShader*	Vertex;		// 0C4
	NiD3DPixelShader*	Pixel;		// 0C8
};
static_assert(sizeof(BSImageSpaceShader) == 0xCC);

class WaterShaderHeightMap : public BSImageSpaceShader {
public:
	UInt32				Unk0CC;		// 0CC
};
static_assert(sizeof(WaterShaderHeightMap) == 0xD0);

class Ni2DBuffer : public NiObject {
public:
	UInt32				width;	// 008
	UInt32				height;	// 00C
	NiDX92DBufferData*	data;	// 010
};
static_assert(sizeof(Ni2DBuffer) == 0x014);

class NiDepthStencilBuffer : public Ni2DBuffer {};
static_assert(sizeof(NiDepthStencilBuffer) == 0x014);

class NiDX92DBufferData : public NiRefObject {
public:
	virtual void		func02();												// 02
	virtual void		func03();												// 03
	virtual void		func04();												// 04
	virtual	void		func05();												// 05
	virtual	void		func06();												// 06
	virtual void		func07();												// 07
	virtual void		func08();												// 08
	virtual void		func09();												// 09
	virtual void		func0A();												// 0A
	virtual void		func0B();												// 0B
	virtual void		func0C();												// 0C
	virtual void		func0D();												// 0D
	virtual void		func0E();												// 0E
	virtual void		func0F();												// 0F
	virtual void		func10();												// 10
	virtual void		func11();												// 11
	virtual void		func12();												// 12
	virtual void		func13();												// 13
	virtual void		func14();												// 14
	virtual void		func15();												// 15
	virtual void		func16();												// 16
	virtual void		func17();												// 17
	virtual void		func18();												// 18
	virtual void		func19();												// 19
	virtual void		func1A();												// 1A

	Ni2DBuffer*			ParentData;												// 08
	UInt32				Unk0C;													// 0C
	UInt32				Unk10;													// 10
	IDirect3DSurface9*	Surface;												// 14
};

class NiRenderTargetGroup : public NiObject {
public:
	virtual UInt32					GetWidth(UInt32 Index);											// 23
	virtual UInt32					GetHeight(UInt32 Index);										// 24
	virtual UInt32					GetDepthStencilWidth();											// 25
	virtual UInt32					GetDepthStencilHeight();										// 26
	virtual const NiPixelFormat*	GetPixelFormat(UInt32 Index);									// 27
	virtual const NiPixelFormat*	GetDepthStencilPixelFormat();									// 28
	virtual UInt32					GetBufferCount();											// 29
	virtual bool					AttachBuffer(Ni2DBuffer* Buffer, UInt32 Index);					// 2A
	virtual bool					AttachDepthStencilBuffer(NiDepthStencilBuffer* DepthBuffer);	// 2B
	virtual Ni2DBuffer*				GetBuffer(UInt32 Index);										// 2C
	virtual NiDepthStencilBuffer*	GetDepthStencilBuffer();										// 2D
	virtual void*					GetRendererData();												// 2E
	virtual void					SetRendererData(void* RendererData);							// 2F
	virtual void*					GetRenderTargetData(UInt32 RenderTargetIndex);					// 30
	virtual void*					GetDepthStencilBufferRendererData();							// 31
	virtual void					func32();													// 32
	virtual void					func33();													// 33

	UInt32							Unk08;						// 08
	Ni2DBuffer*						RenderTargets[4];			// 0C
	UInt32							numRenderTargets;			// 1C
	NiDepthStencilBuffer*			DepthStencilBuffer;			// 20
	void*							RenderData;					// 24
};
static_assert(sizeof(NiRenderTargetGroup) == 0x28);

class NiDX9RenderState : public NiRefObject {
public:
	virtual void							UpdateRenderState(const NiPropertyState* pkNew);// 02
	virtual void							SetAlpha(void* AlphaProperty);					// 03
	virtual void							SetDither(void* DitherProperty);				// 04
	virtual void							SetFog(NiFogProperty* FogProperty);				// 05
	virtual void							SetMaterial(void* u1);							// 06
	virtual void							SetShadeMode(void* ShadeProperty);				// 07
	virtual void							SetSpecular(void* SpecularProperty);			// 08
	virtual void							SetStencil(void* StencilProperty);				// 09
	virtual void							SetWireframe(void* WireframeProperty);			// 0A
	virtual void 							SetZBuffer(void* ZBufferProperty);				// 0B
	virtual void							RestoreAlpha();									// 0C
	virtual void							SetVertexBlending(UInt16 u1);					// 0D
	virtual void							SetNormalization(void* u1);						// 0E
	virtual void							func_0F(UInt32 u1);								// 0F
	virtual float							GetVar088();									// 10
	virtual void							func_11(float u1);								// 11
	virtual void							func_12(float u1, float u2);					// 12
	virtual BOOL							func_13();										// 13
	virtual void							func_14(BOOL u1);								// 14
	virtual void							func_15(UInt32 u1);								// 15
	virtual void							InitializeRenderStates();						// 16
	virtual void							BackUpAllStates();								// 17
	virtual void 							RestoreRenderState(D3DRENDERSTATETYPE state);	// 18
	virtual void							RestoreAllRenderStates();						// 19
	virtual void 							SetRenderState(D3DRENDERSTATETYPE state, UInt32 value, UInt8 Arg3, UInt8 Arg4);					// 1A
	virtual UInt32							GetRenderState(D3DRENDERSTATETYPE state);														// 1B
	virtual void							ClearPixelShaders();																			// 1C
	virtual void							BackUpPixelShader();																			// 1D
	virtual void							func_1E();																						// 1E
	virtual void							SetPixelShader(IDirect3DPixelShader9* PixelShader, UInt8 BackUp);								// 1F
	virtual IDirect3DPixelShader9*			GetPixelShader();																				// 20
	virtual	void							RestorePixelShader();																			// 21
	virtual void							RemovePixelShader(IDirect3DPixelShader9* PixelShader);											// 22
	virtual void							SetVertexShader(IDirect3DVertexShader9* pShader, UInt8 BackUp);									// 23
	virtual IDirect3DVertexShader9*			GetVertexShader();																				// 24
	virtual void							RestoreVertexShader();																			// 25
	virtual void							RemoveVertexShader(IDirect3DVertexShader9* pShader);											// 26
	virtual void							SetFVF(UInt32 FVF, UInt8 BackUp);																// 27
	virtual UInt32							GetFVF();																						// 28
	virtual void							RestoreFVF();																					// 29
	virtual void							ClearFVF(UInt32 FVF);																			// 2A
	virtual void							SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl, UInt8 BackUp);							// 2B
	virtual IDirect3DVertexDeclaration9*	GetVertexDeclaration();																			// 2C
	virtual void							RestoreVertexDeclaration();																		// 2D
	virtual void							ClearVertexDeclatation(IDirect3DVertexDeclaration9* pDecl);										// 2E
	virtual void							func_02F();																						// 2F
	virtual void 							SetTextureStageState(UInt32 Stage, D3DTEXTURESTAGESTATETYPE Type, UInt32 Value, UInt8 BackUp);	// 30
	virtual UInt32							GetTextureStageState(UInt32 Stage, D3DTEXTURESTAGESTATETYPE Type);								// 31
	virtual void							func_032();																						// 32
	virtual HRESULT							SetSamplerState(UInt32 Sampler, D3DSAMPLERSTATETYPE Type, UInt32 Value, UInt8 BackUp);			// 33
	virtual UInt32 							GetSamplerState(UInt32 Sampler, D3DSAMPLERSTATETYPE Type);										// 34
	virtual void							RestoreSamplerState(UInt32 Sampler, D3DSAMPLERSTATETYPE Type);									// 35
	virtual void							ClearTextureList();																				// 36
	virtual void							SetTexture(UInt32 Sampler, IDirect3DBaseTexture9* pTexture);									// 37
	virtual IDirect3DBaseTexture9*			GetTexture(UInt32 Sampler);																		// 38
	virtual void							RemoveTexture(IDirect3DBaseTexture9* pTexture);													// 39
	virtual void							SetSoftwareVertexProcessing(UInt8 bSoftware);													// 3A
	virtual UInt8							GetSoftwareVertexProcessing();																	// 3B
	virtual void							SetVar_0FF4(UInt8 u1);																			// 3C
	virtual UInt8							GetVar_0FF4();																					// 3D
	virtual void							SetVar_0FF5(UInt8 u1);																			// 3E
	virtual UInt8							GetVar_0FF5();																					// 3F
	virtual void							Reset();																						// 40
	virtual void							func_041();																						// 41
	
	struct NiRenderStateSetting {
		UInt32 CurrentValue;
		UInt32 PreviousValue;
	};

	UInt32							Flags;							// 0008
	UInt32							unk000C[(0x0074 - 0x000C) >> 2];// 000C
	NiAlphaProperty*				DisabledAlphaProperty;			// 0074
	float							CameraNear;						// 0078
	float							CameraFar;						// 007C
	float							CameraDepthRange;				// 0080
	float							MaxFogFactor;					// 0084
	float							MaxFogValue;					// 0088
	NiColor							CurrentFogColor;				// 008C
	UInt32                          Unk98[23];						// 0098
	UInt32							LeftHanded;						// 00F4
	UInt32							Unk0F8[10];						// 00F8
	NiRenderStateSetting			RenderStateSettings[256];		// 0120
	UInt32							Unk0920[64];					// 0920
	NiRenderStateSetting			TextureStageStateSettings[128];	// 0A20
	NiRenderStateSetting			SamplerStateSettings[80];		// 0E20
	UInt32							unk10A0[20];					// 10A0
	NiDX9ShaderConstantManager*		ShaderConstantManager;			// 10F0
	UInt8							ForceNormalizeNormals;			// 10F4
	UInt8							InternalNormalizeNormals;		// 10F5
	UInt8							UsingSoftwareVP;				// 10F6
	UInt8							Declaration;					// 10F7
	IDirect3DDevice9*				Device;							// 10F8
	NiDX9Renderer*					Renderer;						// 10FC
	UInt32							unk1000[(0x1018 - 0x1000) >> 2];// 1100
	D3DCAPS9						Caps;							// 1118
};
static_assert(offsetof(NiDX9RenderState, Device) == 0x10F8);
static_assert(sizeof(NiDX9RenderState) == 0x1248);

//class NiRenderer : public NiObject {
//public:
//	enum ClearFlags {
//		kClear_BACKBUFFER = 0x1,
//		kClear_STENCIL = 0x2,
//		kClear_ZBUFFER = 0x4,
//		kClear_NONE = 0,
//		kClear_ALL = kClear_BACKBUFFER | kClear_STENCIL | kClear_ZBUFFER
//	};
//
//	virtual void			Unk_23();
//	virtual void			Unk_24();
//	virtual void			Unk_25();
//	virtual void			Unk_26();
//	virtual void			Unk_27();
//	virtual void			Unk_28();
//	virtual void			Unk_29();
//	virtual void			Unk_2A();
//	virtual void			Unk_2B();
//	virtual void			Unk_2C();
//	virtual void			Unk_2D();
//	virtual void			Unk_2E();
//	virtual void			Unk_2F();
//	virtual void			Unk_30();
//	virtual void			Unk_31();
//	virtual NiRenderTargetGroup* GetDefaultRT();	// get back buffer rt
//	virtual NiRenderTargetGroup* GetCurrentRT();	// get currentRTGroup
//	virtual void			Unk_34();
//	virtual void			Unk_35();
//	virtual void			Unk_36();
//	virtual void			Unk_37();
//	virtual void 			Unk_38();
//	virtual void 			Unk_39();
//	virtual void			Unk_3A();
//	virtual void			Unk_3B();
//	virtual void			PurgeGeometry(NiGeometryData* geo);
//	virtual void			PurgeMaterial(NiMaterialProperty* material);
//	virtual void			PurgeEffect(NiDynamicEffect* effect);
//	virtual void			PurgeScreenTexture();
//	virtual void			PurgeSkinPartition(NiSkinPartition* skinPartition);
//	virtual void			PurgeSkinInstance(NiSkinInstance* skinInstance);
//	virtual void			Unk_42();
//	virtual bool			Unk_43();
//	virtual void			Unk_44();
//	virtual bool			FastCopy(void* src, void* dst, RECT* srcRect, SInt32 xOffset, SInt32 yOffset);
//	virtual bool			Copy(void* src, void* dst, RECT* srcRect, RECT* dstRect, UInt32 filterMode);
//	virtual void			Unk_47();
//	virtual bool			Unk_48(void* arg);
//	virtual void			Unk_49();
//	virtual void			Unk_4A(float arg);
//	virtual void 			Unk_4B(UInt32 size);
//	virtual void			Unk_4C(UInt32 arg0, UInt32 arg1);
//	virtual void			Unk_4D(UInt32 arg0, UInt32 arg1);
//	virtual void			Unk_4E(void* buf);
//	virtual void			CreateSourceTexture(NiSourceTexture* texture);
//	virtual bool			CreateRenderedTexture(NiRenderedTexture* arg);
//	virtual bool			CreateSourceCubeMap(NiSourceCubeMap* arg);
//	virtual bool			CreateRenderedCubeMap(NiRenderedCubeMap* arg);
//	virtual bool			CreateDynamicTexture(void* arg);
//	virtual void			Unk_54();
//	virtual bool			CreateDepthStencil(NiDepthStencilBuffer* arg, void* textureFormat);
//	virtual void			Unk_56();
//	virtual void			Unk_57();
//	virtual void			Unk_58();
//	virtual void			Unk_59();
//	virtual void			Unk_5A();
//	virtual void			Unk_5B();
//	virtual void			Unk_5C();
//	virtual void			Unk_5D();
//	virtual void			Unk_5E();
//	virtual bool			BeginScene();
//	virtual bool			EndScene();
//	virtual void			DisplayScene();
//	virtual void			Clear(float* rect, UInt32 flags);
//	virtual void			SetupCamera(NiPoint3* pos, NiPoint3* at, NiPoint3* up, NiPoint3* right, NiFrustum* frustum, float* viewport);
//	virtual void			SetupScreenSpaceCamera(float* viewport);
//	virtual bool			BeginUsingRenderTargetGroup(NiRenderTargetGroup* renderTarget, ClearFlags clearFlags);
//	virtual bool			EndUsingRenderTargetGroup();
//	virtual void			BeginBatch(UInt32 arg0, UInt32 arg1);
//	virtual void			EndBatch();
//	virtual void			BatchRenderShape(void* arg);
//	virtual void			BatchRenderStrips(void* arg);
//	virtual void			RenderTriShape(NiTriShape* obj);
//	virtual void			RenderTriStrips(NiTriStrips* obj);
//	virtual void			RenderTriShape2(NiTriShape* obj);
//	virtual void			RenderTriStrips2(NiTriStrips* obj);
//	virtual void			RenderParticles(NiParticles* obj);
//	virtual void			RenderLines(NiLines* obj);
//	virtual void			RenderScreenTexture();
//
//	UInt32					Unk008[126];				// 008
//	UInt32					SceneState;					// 200
//	UInt32					Unk204;						// 204
//	UInt32					Unk208;						// 208
//	UInt32					Unk20C;						// 20C
//};
//static_assert(sizeof(NiRenderer) == 0x210);
//
//class NiDX9Renderer : public NiRenderer {
//public:
//	enum FrameBufferFormat {
//		FBFMT_UNKNOWN = 0,
//		FBFMT_R8G8B8,
//		FBFMT_A8R8G8B8,
//		FBFMT_X8R8G8B8,
//		FBFMT_R5G6B5,
//		FBFMT_X1R5G5B5,
//		FBFMT_A1R5G5B5,
//		FBFMT_A4R4G4B4,
//		FBFMT_R3G3B2,
//		FBFMT_A8,
//		FBFMT_A8R3G3B2,
//		FBFMT_X4R4G4B4,
//		FBFMT_R16F,
//		FBFMT_G16R16F,
//		FBFMT_A16B16G16R16F,
//		FBFMT_R32F,
//		FBFMT_G32R32F,
//		FBFMT_A32B32G32R32F,
//		FBFMT_NUM
//	};
//
//	enum DepthStencilFormat {
//		DSFMT_UNKNOWN = 0,
//		DSFMT_D16_LOCKABLE = 70,
//		DSFMT_D32 = 71,
//		DSFMT_D15S1 = 73,
//		DSFMT_D24S8 = 75,
//		DSFMT_D16 = 80,
//		DSFMT_D24X8 = 77,
//		DSFMT_D24X4S4 = 79,
//	};
//
//	enum PresentationInterval {
//		PRESENT_INTERVAL_IMMEDIATE = 0,
//		PRESENT_INTERVAL_ONE = 1,
//		PRESENT_INTERVAL_TWO = 2,
//		PRESENT_INTERVAL_THREE = 3,
//		PRESENT_INTERVAL_FOUR = 4,
//		PRESENT_INTERVAL_NUM
//	};
//
//	enum SwapEffect {
//		SWAPEFFECT_DEFAULT,
//		SWAPEFFECT_DISCARD,
//		SWAPEFFECT_FLIP,
//		SWAPEFFECT_COPY,
//		SWAPEFFECT_NUM
//	};
//
//	enum FrameBufferMode {
//		FBMODE_DEFAULT,
//		FBMODE_LOCKABLE,
//		FBMODE_MULTISAMPLES_2 = 0x00010000,
//		FBMODE_MULTISAMPLES_3 = 0x00020000,
//		FBMODE_MULTISAMPLES_4 = 0x00030000,
//		FBMODE_MULTISAMPLES_5 = 0x00040000,
//		FBMODE_MULTISAMPLES_6 = 0x00050000,
//		FBMODE_MULTISAMPLES_7 = 0x00060000,
//		FBMODE_MULTISAMPLES_8 = 0x00070000,
//		FBMODE_MULTISAMPLES_9 = 0x00080000,
//		FBMODE_MULTISAMPLES_10 = 0x00090000,
//		FBMODE_MULTISAMPLES_11 = 0x000a0000,
//		FBMODE_MULTISAMPLES_12 = 0x000b0000,
//		FBMODE_MULTISAMPLES_13 = 0x000c0000,
//		FBMODE_MULTISAMPLES_14 = 0x000d0000,
//		FBMODE_MULTISAMPLES_15 = 0x000e0000,
//		FBMODE_MULTISAMPLES_16 = 0x000f0000,
//		FBMODE_MULTISAMPLES_NONMASKABLE = 0x80000000,
//		FBMODE_QUALITY_MASK = 0x0000FFFF,
//		FBMODE_NUM = 18
//	};
//
//	enum RefreshRate {
//		REFRESHRATE_DEFAULT = 0
//	};
//
//	void							SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE State, DWORD Value);
//	void							PackGeometryBuffer(NiGeometryBufferData* GeoData, NiGeometryData* ModelData, NiSkinInstance* SkinInstance, NiD3DShaderDeclaration* ShaderDeclaration);
//	void							PackSkinnedGeometryBuffer(NiGeometryBufferData* GeoData, NiGeometryData* ModelData, NiSkinInstance* SkinInstance, NiSkinPartition::Partition* Partition, NiD3DShaderDeclaration* ShaderDeclaration);
//	void							CalculateBoneMatrixes(NiSkinInstance* SkinInstance, NiTransform* WorldTrasform);
//
//	UInt32							pad210[(0x288 - 0x210) >> 2];	// 210
//	IDirect3DDevice9*				device;							// 288
//	D3DCAPS9						caps;							// 28C
//	HANDLE							deviceWindow;					// 3BC
//	HANDLE							focusWindow;					// 3C0
//	char							rendererInfo[0x200];			// 3C4
//	UInt32							adapterIdx;						// 5C4
//	UInt32							d3dDevType;						// 5C8 - D3DDEVTYPE
//	UInt32							d3dDevFlags;					// 5CC - D3DCREATE
//	UInt8							softwareVertexProcessing;		// 5D0 - !D3DCREATE_HARDWARE_VERTEXPROCESSING
//	UInt8							mixedVertexProcessing;			// 5D1 - D3DCREATE_MIXED_VERTEXPROCESSING
//	UInt8							pad5D2[2];						// 5D2
//	UInt32							Unk5D4[15];						// 5D4
//	NiTMap<void*, void*>			PrePackObjects;					// 610 - NiTPointerMap <NiVBBlock *, NiDX9Renderer::PrePackObject *>
//	UInt32							Unk620[153];					// 620
//	NiRenderTargetGroup*			defaultRTGroup;					// 884 - back buffer
//	NiRenderTargetGroup*			currentRTGroup;					// 888
//	NiRenderTargetGroup*			currentscreenRTGroup;			// 88C
//	NiTMap<HWND*, NiRenderTargetGroup*>	screenRTGroups;					// 890 - NiTPointerMap <HWND *, NiPointer <NiRenderTargetGroup> >
//	UInt32							Unk8A0[6];						// 8A0
//	NiDX9RenderState*				renderState;					// 8B8
//	UInt32							Unk8BC[33];						// 8BC
//	D3DXMATRIXA16					worldMatrix;				// 940
//	D3DXMATRIX						viewMatrix;					// 980
//	D3DXMATRIX						projMatrix;					// 9C0
//	D3DXMATRIX						UnkMatrix;					// A00
//	D3DXMATRIX						invViewMatrix;				// A40
//	UInt32							UnkA80[6];					// A80
//	UInt32							width;						// A98
//	UInt32							height;						// A9C
//	UInt32							flags;						// AA0
//	UInt32							windowDevice;				// AA4
//	UInt32							windowFocus;				// AA8
//	UInt32							adapterType;				// AAC
//	UInt32							deviceType;					// AB0
//	FrameBufferFormat				frameBufferFormat;			// AB4
//	DepthStencilFormat				depthStencilFormat;			// AB8
//	PresentationInterval			presentationInterval;		// ABC
//	SwapEffect						swapEffect;					// AC0
//	FrameBufferMode					frameBufferMode;			// AC4
//	UInt32							backBufferCount;			// AC8
//	RefreshRate						refreshRate;				// ACC
//	UInt32							UnkAD0[44];					// AD0
//};
//static_assert(sizeof(NiDX9Renderer) == 0xB80);

class NiRenderer : public NiObject {
public:
	enum ClearFlags {
		kClear_BACKBUFFER = 0x1,
		kClear_STENCIL = 0x2,
		kClear_ZBUFFER = 0x4,
		kClear_NONE = 0,
		kClear_ALL = kClear_BACKBUFFER | kClear_STENCIL | kClear_ZBUFFER
	};

	virtual void			Unk_23();
	virtual void			Unk_24();
	virtual void			Unk_25();
	virtual void			Unk_26();
	virtual void			Unk_27();
	virtual void			Unk_28();
	virtual void			SetDepthClear(float afValue);
	virtual float			GetDepthClear() const;
	virtual void			Unk_2B();
	virtual void			Unk_2C();
	virtual void			Unk_2D();
	virtual void			Unk_2E();
	virtual void			Unk_2F();
	virtual void			Unk_30();
	virtual void			Unk_31();
	virtual NiRenderTargetGroup* GetDefaultRT();	// get back buffer rt
	virtual NiRenderTargetGroup* GetCurrentRT();	// get currentRTGroup
	virtual void			Unk_34();
	virtual void			Unk_35();
	virtual void			Unk_36();
	virtual void			Unk_37();
	virtual void 			Unk_38();
	virtual void 			Unk_39();
	virtual void			Unk_3A();
	virtual void			Unk_3B();
	virtual void			PurgeGeometry(NiGeometryData* geo);
	virtual void			PurgeMaterial(NiMaterialProperty* material);
	virtual void			PurgeEffect(NiDynamicEffect* effect);
	virtual void			PurgeScreenTexture();
	virtual void			PurgeSkinPartition(NiSkinPartition* skinPartition);
	virtual void			PurgeSkinInstance(NiSkinInstance* skinInstance);
	virtual void			Unk_42();
	virtual bool			Unk_43();
	virtual void			Unk_44();
	virtual bool			FastCopy(void* src, void* dst, RECT* srcRect, SInt32 xOffset, SInt32 yOffset);
	virtual bool			Copy(void* src, void* dst, RECT* srcRect, RECT* dstRect, UInt32 filterMode);
	virtual void			Unk_47();
	virtual bool			Unk_48(void* arg);
	virtual void			Unk_49();
	virtual void			Unk_4A(float arg);
	virtual void 			Unk_4B(UInt32 size);
	virtual void			Unk_4C(UInt32 arg0, UInt32 arg1);
	virtual void			Unk_4D(UInt32 arg0, UInt32 arg1);
	virtual void			Unk_4E(void* buf);
	virtual void			CreateSourceTexture(NiSourceTexture* texture);
	virtual bool			CreateRenderedTexture(NiRenderedTexture* arg);
	virtual bool			CreateSourceCubeMap(NiSourceCubeMap* arg);
	virtual bool			CreateRenderedCubeMap(NiRenderedCubeMap* arg);
	virtual bool			CreateDynamicTexture(void* arg);
	virtual void			Unk_54();
	virtual bool			CreateDepthStencil(NiDepthStencilBuffer* arg, void* textureFormat);
	virtual void			Unk_56();
	virtual void			Unk_57();
	virtual void			Unk_58();
	virtual void			Unk_59();
	virtual void			Unk_5A();
	virtual void			Unk_5B();
	virtual void			Unk_5C();
	virtual void			Unk_5D();
	virtual void			Unk_5E();
	virtual bool			BeginScene();
	virtual bool			EndScene();
	virtual void			DisplayScene();
	virtual void			Clear(float* rect, UInt32 flags);
	virtual void			SetupCamera(NiPoint3* pos, NiPoint3* at, NiPoint3* up, NiPoint3* right, NiFrustum* frustum, float* viewport);
	virtual void			SetupScreenSpaceCamera(float* viewport);
	virtual bool			BeginUsingRenderTargetGroup(NiRenderTargetGroup* renderTarget, ClearFlags clearFlags);
	virtual bool			EndUsingRenderTargetGroup();
	virtual void			BeginBatch(UInt32 arg0, UInt32 arg1);
	virtual void			EndBatch();
	virtual void			BatchRenderShape(void* arg);
	virtual void			BatchRenderStrips(void* arg);
	virtual void			RenderTriShape(NiTriShape* obj);
	virtual void			RenderTriStrips(NiTriStrips* obj);
	virtual void			RenderTriShape2(NiTriShape* obj);
	virtual void			RenderTriStrips2(NiTriStrips* obj);
	virtual void			E70140(void*);
	virtual void			RenderParticles(NiParticles* obj);
	virtual void			RenderLines(NiLines* obj);
	virtual void			RenderScreenTexture();

	BSShaderAccumulator* m_spBSShaderAccum;
	NiPropertyState* m_pkCurrProp;
	NiDynamicEffectState* m_pkCurrEffects;
	NiShader* m_spErrorShader;
	void* m_spInitialDefaultMaterial;
	void* m_spCurrentDefaultMaterial;
	void* m_spAccum;
	float					unk024;
	float					unk028;
	NiRenderer* ms_pkRenderer;
	NiRect<float>			m_kDisplaySafeZone;
	char					unk040[62];
	UInt32					m_kRendererLock[8];
	char					unk0A0[94];
	UInt32					m_kPrecacheCriticalSection[8];
	char					unk120[95];
	UInt32					m_kSourceDataCriticalSection[8];
	char					unk1AC[92];
	UInt32					m_eSavedFrameState;
	UInt32					SceneState;					// 200
	UInt32					m_uiFrameID;
	bool					m_bRenderTargetGroupActive;
	bool					m_bBatchRendering;
	int						unk20C[29];
};
static_assert(sizeof(NiRenderer) == 0x280);

class NiDX9Renderer : public NiRenderer {
public:
	enum FrameBufferFormat {
		FBFMT_UNKNOWN = 0,
		FBFMT_R8G8B8,
		FBFMT_A8R8G8B8,
		FBFMT_X8R8G8B8,
		FBFMT_R5G6B5,
		FBFMT_X1R5G5B5,
		FBFMT_A1R5G5B5,
		FBFMT_A4R4G4B4,
		FBFMT_R3G3B2,
		FBFMT_A8,
		FBFMT_A8R3G3B2,
		FBFMT_X4R4G4B4,
		FBFMT_R16F,
		FBFMT_G16R16F,
		FBFMT_A16B16G16R16F,
		FBFMT_R32F,
		FBFMT_G32R32F,
		FBFMT_A32B32G32R32F,
		FBFMT_NUM
	};

	enum DepthStencilFormat {
		DSFMT_UNKNOWN = 0,
		DSFMT_D16_LOCKABLE = 70,
		DSFMT_D32 = 71,
		DSFMT_D15S1 = 73,
		DSFMT_D24S8 = 75,
		DSFMT_D16 = 80,
		DSFMT_D24X8 = 77,
		DSFMT_D24X4S4 = 79,
	};

	enum PresentationInterval {
		PRESENT_INTERVAL_IMMEDIATE = 0,
		PRESENT_INTERVAL_ONE = 1,
		PRESENT_INTERVAL_TWO = 2,
		PRESENT_INTERVAL_THREE = 3,
		PRESENT_INTERVAL_FOUR = 4,
		PRESENT_INTERVAL_NUM
	};

	enum SwapEffect {
		SWAPEFFECT_DEFAULT,
		SWAPEFFECT_DISCARD,
		SWAPEFFECT_FLIP,
		SWAPEFFECT_COPY,
		SWAPEFFECT_NUM
	};

	enum FrameBufferMode {
		FBMODE_DEFAULT,
		FBMODE_LOCKABLE,
		FBMODE_MULTISAMPLES_2 = 0x00010000,
		FBMODE_MULTISAMPLES_3 = 0x00020000,
		FBMODE_MULTISAMPLES_4 = 0x00030000,
		FBMODE_MULTISAMPLES_5 = 0x00040000,
		FBMODE_MULTISAMPLES_6 = 0x00050000,
		FBMODE_MULTISAMPLES_7 = 0x00060000,
		FBMODE_MULTISAMPLES_8 = 0x00070000,
		FBMODE_MULTISAMPLES_9 = 0x00080000,
		FBMODE_MULTISAMPLES_10 = 0x00090000,
		FBMODE_MULTISAMPLES_11 = 0x000a0000,
		FBMODE_MULTISAMPLES_12 = 0x000b0000,
		FBMODE_MULTISAMPLES_13 = 0x000c0000,
		FBMODE_MULTISAMPLES_14 = 0x000d0000,
		FBMODE_MULTISAMPLES_15 = 0x000e0000,
		FBMODE_MULTISAMPLES_16 = 0x000f0000,
		FBMODE_MULTISAMPLES_NONMASKABLE = 0x80000000,
		FBMODE_QUALITY_MASK = 0x0000FFFF,
		FBMODE_NUM = 18
	};

	enum RefreshRate {
		REFRESHRATE_DEFAULT = 0
	};

	enum TexFormat {
		TEX_RGB555 = 0x0,
		TEX_RGB565 = 0x1,
		TEX_RGB888 = 0x2,
		TEX_RGBA5551 = 0x3,
		TEX_RGBA4444 = 0x4,
		TEX_RGBA8888 = 0x5,
		TEX_PAL8 = 0x6,
		TEX_PALA8 = 0x7,
		TEX_DXT1 = 0x8,
		TEX_DXT3 = 0x9,
		TEX_DXT5 = 0xA,
		TEX_BUMP88 = 0xB,
		TEX_BUMPLUMA556 = 0xC,
		TEX_BUMPLUMA888 = 0xD,
		TEX_L8 = 0xE,
		TEX_A8 = 0xF,
		TEX_R16F = 0x10,
		TEX_RG32F = 0x11,
		TEX_RGBA64F = 0x12,
		TEX_R32F = 0x13,
		TEX_RG64F = 0x14,
		TEX_RGBA128F = 0x15,
		TEX_DEFAULT = 0x16,
		TEX_NUM = 0x16,
	};

	enum DeviceDesc
	{
		DEVDESC_PURE = 0x0,
		DEVDESC_HAL_HWVERTEX = 0x1,
		DEVDESC_HAL_MIXEDVERTEX = 0x2,
		DEVDESC_HAL_SWVERTEX = 0x3,
		DEVDESC_REF = 0x4,
		DEVDESC_REF_HWVERTEX = 0x5,
		DEVDESC_REF_MIXEDVERTEX = 0x6,
		DEVDESC_NUM = 0x7,
	};

	class PrePackObject {
	public:
		NiGeometryData* m_pkData;
		NiSkinInstance* m_pkSkin;
		NiSkinPartition::Partition* m_pkPartition;
		NiD3DShaderDeclaration* m_pkShaderDecl;
		UInt32							m_uiBonesPerPartition;
		UInt32							m_uiBonesPerVertex;
		NiGeometryBufferData* m_pkBuffData;
		UInt32							m_uiStream;
		PrePackObject* m_pkNext;
	};

	void								SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE State, DWORD Value);
	void								PackGeometryBuffer(NiGeometryBufferData* GeoData, NiGeometryData* ModelData, NiSkinInstance* SkinInstance, NiD3DShaderDeclaration* ShaderDeclaration);
	void								PackSkinnedGeometryBuffer(NiGeometryBufferData* GeoData, NiGeometryData* ModelData, NiSkinInstance* SkinInstance, NiSkinPartition::Partition* Partition, NiD3DShaderDeclaration* ShaderDeclaration);
	void								CalculateBoneMatrixes(NiSkinInstance* SkinInstance, NiTransform* WorldTrasform);

	LPDIRECT3DVERTEXDECLARATION9		hParticleVertexDecls[2];
	IDirect3DDevice9* device;
	D3DCAPS9							m_kD3DCaps9;
	HWND								m_kWndDevice;
	HWND								m_kWndFocus;
	char								m_acDriverDesc[512];
	UInt32								adapterIdx;						// 5C4
	D3DDEVTYPE							d3dDevType;						// 5C8 - D3DDEVTYPE
	UInt32								m_uiBehaviorFlags;
	D3DFORMAT							m_eAdapterFormat;
	bool								m_bSWVertexCapable;
	bool								m_bSWVertexSwitchable;
	const NiDX9AdapterDesc* m_pkAdapterDesc;
	const NiDX9DeviceDesc* m_pkDeviceDesc;
	UInt32								m_uiBackground;
	float								m_fZClear;
	UInt32								m_uiStencilClear;
	UInt32								m_uiRendFlags;
	char								m_acBehavior[32];
	NiTMap<void*, PrePackObject*>		PrePackObjects;					// 610 - NiTPointerMap <NiVBBlock *, NiDX9Renderer::PrePackObject *>
	void* m_pkBatchHead;
	void* m_pkBatchTail;
	NiPropertyState* m_pkBatchedPropertyState;
	NiDynamicEffectState* m_pkBatchedEffectState;
	NiD3DShaderInterface* m_spBatchedShader;
	NiPoint3							m_kCamRight;
	NiPoint3							m_kCamUp;
	NiPoint3							m_kModelCamRight;
	NiPoint3							m_kModelCamUp;
	NiBound								m_kWorldBound;
	const NiBound						m_kDefaultBound;
	float								m_fNearDepth;
	float								m_fDepthRange;
	D3DXMATRIX							m_kD3DIdentity;
	D3DVIEWPORT9						m_kD3DPort;
	UInt32								m_uiHWBones;
	UInt32								m_uiMaxStreams;
	UInt32								m_uiMaxPixelShaderVersion;
	UInt32								m_uiMaxVertexShaderVersion;
	bool								m_bMipmapCubeMaps;
	bool								m_bDynamicTexturesCapable;
	UInt32								m_uiResetCounter;
	bool								m_bDeviceLost;
	NiPixelFormat* m_aapkTextureFormats[4][22];
	NiPixelFormat* m_apkDefaultTextureFormat[4];
	NiPixelData* m_aspDefaultTextureData[4];
	TexFormat							m_eReplacementDataFormat;
	NiRenderTargetGroup* defaultRTGroup;					// 884 - back buffer
	NiRenderTargetGroup* currentRTGroup;					// 888
	NiRenderTargetGroup* currentscreenRTGroup;			// 88C
	NiTMap<HWND*, NiRenderTargetGroup*>	screenRTGroups;					// 890 - NiTPointerMap <HWND *, NiPointer <NiRenderTargetGroup> >
	UInt32								m_uiMaxNumRenderTargets;
	bool								m_bIndependentBitDepths;
	bool								m_bMRTPostPixelShaderBlending;
	UInt32								Unk8A0[4];						// 8A0
	NiDX9RenderState* renderState;					// 8B8
	UInt32								Unk8BC[33];						// 8BC
	D3DXMATRIXA16						worldMatrix;				// 940
	D3DXMATRIX							viewMatrix;					// 980
	D3DXMATRIX							projMatrix;					// 9C0
	D3DXMATRIX							viewProjMatrix;			    // A00
	D3DXMATRIX							invViewMatrix;				// A40
	UInt32								UnkA80[3];					// A80
	UInt16								m_usNumScreenTextureVerts;
	UInt16* m_pusScreenTextureIndices;
	UInt32								m_uiNumScreenTextureIndices;
	UInt32								width;						// A98
	UInt32								height;						// A9C
	UInt32								flags;						// AA0
	HWND								windowDevice;				// AA4
	HWND								windowFocus;				// AA8
	UInt32								adapterType;				// AAC
	DeviceDesc							deviceType;					// AB0
	FrameBufferFormat					frameBufferFormat;			// AB4
	DepthStencilFormat					depthStencilFormat;			// AB8
	PresentationInterval				presentationInterval;		// ABC
	SwapEffect							swapEffect;					// AC0
	FrameBufferMode						frameBufferMode;			// AC4
	UInt32								backBufferCount;			// AC8
	RefreshRate							refreshRate;				// ACC
	bool								unkAD0;                     // AD0
	UInt32								m_kResetNotifyFuncsp[4];
	UInt32								m_kResetNotifyFuncData[4];
	UInt32								m_kLostDeviceNotifyFuncs[4];
	UInt32								m_kLostDeviceNotifyFuncData[4];
	NiTMap<D3DFORMAT, NiPixelFormat*>	m_kDepthStencilFormats;
	NiFrustum							m_kCachedFrustum;
	NiRect<float>						m_kCachedPort;
	void* unkB50[12];

	static NiDX9Renderer* GetSingleton() { return *(NiDX9Renderer**)0x11C73B4; };
};
static_assert(sizeof(NiDX9Renderer) == 0xB80);

class NiControllerSequence : public NiObject {
public:
	virtual void	Unk_23();

	struct ControlledBlock {
		void*	unk00;		// 00 NiInterpolator*
		void*	unk04;		// 04 NiMultiTargetTransformController*
		UInt32	unk08;		// 08
		UInt8	unk0C;		// 0C
		UInt8	pad0C[3];
	};

	const char*				sequenceName;			// 08
	UInt32					numControlledBlocks;	// 0C
	UInt32					arrayGrowBy;			// 10
	ControlledBlock**		controlledBlocks;		// 14
	const char**			unkNodeName;			// 18
	float					weight;					// 1C
	NiTextKeyExtraData*		textKeyData;			// 20
	UInt32					cycleType;				// 24
	float					frequency;				// 28
	float					startTime;				// 2C
	float					stopTime;				// 30
	float					flt34;					// 34
	float					flt38;					// 38
	float					flt3C;					// 3C
	NiControllerManager*	manager;				// 40
	UInt32					unk44;					// 44
	UInt32					unk48;					// 48
	float					flt4C;					// 4C
	float					flt50;					// 50
	float					flt54;					// 54
	UInt32					unk58;					// 58
	const char*				rootNodeName;			// 5C
	UInt32					unk60[5];				// 60
};
static_assert(sizeof(NiControllerSequence) == 0x74);

class BSRenderedTexture : public NiObject {
public:
	NiRenderTargetGroup*	RenderTargetGroup;	// 008
	UInt32					unk00C[5];			// 00C
	UInt32					unk020;				// 020
	UInt32					unk024;				// 024
	UInt32					unk028;				// 028
	UInt32					unk02C;				// 02C
	NiRenderedTexture*		RenderedTexture;	// 030
	UInt32					unk034;				// 034
	UInt32					unk038;				// 038
	UInt32					unk03C;				// 03C
};
static_assert(sizeof(BSRenderedTexture) == 0x40);

class NiAlphaProperty : public NiProperty {
public:
	enum AlphaFlags {
        ALPHA_BLEND_MASK    = 0x0001,
        SRC_BLEND_MASK      = 0x001e,
        SRC_BLEND_POS       = 1,
        DEST_BLEND_MASK     = 0x01e0,
        DEST_BLEND_POS      = 5,
        TEST_ENABLE_MASK    = 0x0200,
        TEST_FUNC_MASK      = 0x1c00,
        TEST_FUNC_POS       = 10,
        ALPHA_NOSORTER_MASK = 0x2000
    };

	UInt16	flags;			// 018
	UInt8	alphaTestRef;	// 01A
	UInt8	unk01B;			// 01B
};
static_assert(sizeof(NiAlphaProperty) == 0x01C);

class BSShaderProperty : public NiShadeProperty {
public:
	enum BSShaderType {
		kType_TallGrass		= 0,
		kType_Default		= 1,
		kType_Sky			= 10,
		kType_Skin			= 14,
		kType_Water			= 17,
		kType_Lighting3		= 29,
		kType_Tile			= 32,
		kType_NoLighting	= 33,
	};
	enum BSShaderFlags {
		kFlags_Specular				= 0x00000001,
		kFlags_Skinned				= 0x00000002,
		kFlags_LowDetail			= 0x00000004,
		kFlags_VertexAlpha			= 0x00000008,
		kFlags_Unk1					= 0x00000010, // MultiSpecular?
		kFlags_SinglePass			= 0x00000020,
		kFlags_Empty				= 0x00000040,
		kFlags_EnvMapping			= 0x00000080,
		kFlags_AlphaTexture			= 0x00000100,
		kFlags_Unk2					= 0x00000200,
		kFlags_Facegen				= 0x00000400,
		kFlags_ParallaxShader		= 0x00000800,
		kFlags_Unk3					= 0x00001000,
		kFlags_NoProjShadow			= 0x00002000,
		kFlags_LandscapeTexturing	= 0x00004000,
		kFlags_SimpleRefraction		= 0x00008000,
		kFlags_ComplexRefraction	= 0x00010000,
		kFlags_EyeEnvMapping		= 0x00020000,
		kFlags_Hair					= 0x00040000,
		kFlags_DynamicAlpha			= 0x00080000,
		kFlags_LocalMapHideSecret	= 0x00100000,
		kFlags_WindowsEnvMapping	= 0x00200000,
		kFlags_TreeBillboard		= 0x00400000,
		kFlags_ShadowFrustum		= 0x00800000,
		kFlags_MultipleTexture		= 0x01000000,
		kFlags_RemappableTexture	= 0x02000000,
		kFlags_DecalSinglePass		= 0x04000000,
		kFlags_DynDecalSinglePass	= 0x08000000,
		kFlags_ParallaxOcclusion	= 0x10000000,
		kFlags_ExternalEmittance	= 0x20000000,
		kFlags_ShadowMap			= 0x40000000,
		kFlags_ZBufferTest			= 0x80000000,
	};
	
	bool	IsLightingProperty();
	
	UInt32	Unk020;		// 020
	UInt32	Unk024;		// 024
	float	Unk028;		// 028
	float	Unk02C;		// 02C
	float	Unk030;		// 030
	float	Unk034;		// 034
	UInt32	Unk038;		// 038
	UInt32	Unk03C;		// 03C
	UInt32	Unk040;		// 040
	UInt32	Unk044;		// 044
	UInt32	Unk048;		// 048
	UInt32	Unk04C;		// 04C
	UInt32	Unk050;		// 050
	UInt32	Unk054;		// 054
	UInt32	type;		// 058
	float	Unk05C;		// 05C
};
static_assert(sizeof(BSShaderProperty) == 0x60);

class WaterShaderProperty : BSShaderProperty
{
public:
    struct VarAmounts
    {
      float unk;
      float fWaterReflectivityAmt;
      float fWaterOpacity;
      float fWaterDistortionAmt;
    };

  UInt8 byte60;
  UInt8 byte61;
  UInt8 byte62;
  bool bDepth;
  UInt32 dword64;
  UInt32 dword68;
  float blendRadiusX;
  float blendRadiusY;
  float fogPower;
  float fog78;
  UInt8 byte7C;
  UInt8 byte7D;
  UInt8 byte7E;
  bool isUseDefaultWater;
  bool bReflect;
  UInt8 bRefract;
  UInt8 UV;
  UInt8 byte83;
  UInt32 dword84;
  NiColorAlpha shallowColor;
  NiColorAlpha deepColor;
  NiColorAlpha reflectionColor;
  WaterShaderProperty::VarAmounts Vars;
  float floatC8;
  float floatCC;
  float blendRadiusZ;
  float floatD4;
  NiVector4 depthData;
  float floatE8;
  float floatEC;
  float floatF0;
  float floatF4;
  float floatF8;
  float floatFC;
  float fresnelZ;
  float fresnelW;
  float float108;
  float float10C;
  float float110;
  float float114;
  UInt8 fWaterFresnelTerm[4];
  float fWaterNoise;
  float fFogAmount;
  float texScale;
  UInt32 dword128;
  UInt32 dword12C;
  UInt32 dword130;
  NiSourceTexture *noiseTexture;
  BSRenderedTexture *noDepth;
  BSRenderedTexture *reflections;
  BSRenderedTexture *refractions;
  BSRenderedTexture *depth;
  UInt32 dword148;
  UInt32 dword14C;
};
static_assert(sizeof(WaterShaderProperty) == 0x150);


class BSShaderLightingProperty : public BSShaderProperty {
public:
	UInt32	Unk060;	// 060
	UInt32	Unk064;	// 064
	UInt32	Unk068;	// 068
	float	Unk06C;	// 06C
	UInt32	Unk070;	// 070
	UInt32	Unk074;	// 074
	UInt32	Unk078;	// 078
};
static_assert(sizeof(BSShaderLightingProperty) == 0x7C);

class BSShaderPPLightingProperty : public BSShaderLightingProperty {
public:
	UInt32		Unk07C;	// 07C
	UInt32		Unk080;	// 080
	float		Unk084;	// 084
	float		Unk088;	// 088
	float		Unk08C;	// 08C
	float		Unk090;	// 090
	float		Unk094;	// 094
	float		Unk098;	// 098
	float		Unk09C;	// 09C
	float		Unk0A0;	// 0A0
	UInt32		Unk0A4;	// 0A4
	UInt16		numLandscapeTextures;	// 0A8
	UInt8		pad0A8[2];
	NiTexture** textures[4];			// 0BC - array for texturing: 0 diffuse, 1 normal, 2 glow, 3 unk
	UInt32		Unk0BC;	// 0BC
	UInt32		Unk0C0;	// 0C0
	UInt32		Unk0C4;	// 0C4
	UInt32		Unk0C8;	// 0C8
	UInt32		Unk0CC;	// 0CC
	UInt32		Unk0D0;	// 0D0
	UInt32		Unk0D4;	// 0D4
	UInt32		Unk0D8;	// 0D8
	UInt32		Unk0DC;	// 0DC
	UInt32		Unk0E0;	// 0E0
	UInt32		Unk0E4;	// 0E4
	UInt32		Unk0E8;	// 0E8
	UInt32		Unk0EC;	// 0EC
	UInt32		Unk0F0;	// 0F0
	UInt32		Unk0F4;	// 0F4
	UInt32		Unk0F8;	// 0F8
	UInt32		Unk0FC;	// 0FC
	UInt32		Unk100;	// 100
};
static_assert(sizeof(BSShaderPPLightingProperty) == 0x104);

enum RenderPassTypes {
	BSSM_ZONLY = 0x1,
	BSSM_ZONLY_At = 0x2,
	BSSM_ZONLY_S = 0x3,
	BSSM_ZONLY_TEXEFFECT = 0x4,
	BSSM_ZONLY_TEXEFFECT_S = 0x5,
	BSSM_CONSTALPHA = 0x6,
	BSSM_CONSTALPHA_At = 0x7,
	BSSM_CONSTALPHA_S = 0x8,
	BSSM_AMBIENT_OCCLUSION = 0x9,
	BSSM_3XZONLY = 0xA,
	BSSM_3XZONLY_S = 0xB,
	BSSM_3XZONLY_Vc = 0xC,
	BSSM_3XZONLY_SVc = 0xD,
	BSSM_3XZONLY_TEXEFFECT = 0xE,
	BSSM_3XZONLY_TEXEFFECT_S = 0xF,
	BSSM_DEPTHMAP = 0x10,
	BSSM_DEPTHMAP_At = 0x11,
	BSSM_DEPTHMAP_S = 0x12,
	BSSM_DEPTHMAP_SAt = 0x13,
	BSSM_SELFILLUMALPHABLOCK = 0x14,
	BSSM_SELFILLUMALPHABLOCK_S = 0x15,
	BSSM_GRASS_NOALPHA_DIRONLY_LF = 0x16,
	BSSM_GRASS_NOALPHA_DIRONLY_LV = 0x17,
	BSSM_GRASS_NOALPHA_DIRONLY_LFS = 0x18,
	BSSM_GRASS_NOALPHA_DIRONLY_LVS = 0x19,
	BSSM_GRASS_NOALPHA_DIRONLY_LFP = 0x1A,
	BSSM_GRASS_NOALPHA_DIRONLY_LVP = 0x1B,
	BSSM_GRASS_NOALPHA_DIRONLY_LFSP = 0x1C,
	BSSM_GRASS_NOALPHA_DIRONLY_LVSP = 0x1D,
	BSSM_GRASS_NOALPHA_DIRONLY_LFShp = 0x1E,
	BSSM_GRASS_NOALPHA_DIRONLY_LVShp = 0x1F,
	BSSM_GRASS_NOALPHA_DIRONLY_LFSShp = 0x20,
	BSSM_GRASS_NOALPHA_DIRONLY_LVSShp = 0x21,
	BSSM_GRASS_NOALPHA_DIRONLY_LFPShp = 0x22,
	BSSM_GRASS_NOALPHA_DIRONLY_LVPShp = 0x23,
	BSSM_GRASS_NOALPHA_DIRONLY_LFSPShp = 0x24,
	BSSM_GRASS_NOALPHA_DIRONLY_LVSPShp = 0x25,
	BSSM_GRASS_NOALPHA_DIRONLY_LFB = 0x26,
	BSSM_GRASS_NOALPHA_DIRONLY_LFSB = 0x27,
	BSSM_GRASS_NOALPHA_DIRONLY_LFPB = 0x28,
	BSSM_GRASS_NOALPHA_DIRONLY_LFSPB = 0x29,
	BSSM_GRASS_NOALPHA_1POINT_LF = 0x2A,
	BSSM_GRASS_NOALPHA_1POINT_LV = 0x2B,
	BSSM_GRASS_NOALPHA_1POINT_LFS = 0x2C,
	BSSM_GRASS_NOALPHA_1POINT_LVS = 0x2D,
	BSSM_GRASS_NOALPHA_1POINT_LFP = 0x2E,
	BSSM_GRASS_NOALPHA_1POINT_LVP = 0x2F,
	BSSM_GRASS_NOALPHA_1POINT_LFSP = 0x30,
	BSSM_GRASS_NOALPHA_1POINT_LVSP = 0x31,
	BSSM_GRASS_NOALPHA_1POINT_LFShp = 0x32,
	BSSM_GRASS_NOALPHA_1POINT_LVShp = 0x33,
	BSSM_GRASS_NOALPHA_1POINT_LFSShp = 0x34,
	BSSM_GRASS_NOALPHA_1POINT_LVSShp = 0x35,
	BSSM_GRASS_NOALPHA_1POINT_LFPShp = 0x36,
	BSSM_GRASS_NOALPHA_1POINT_LVPShp = 0x37,
	BSSM_GRASS_NOALPHA_1POINT_LFSPShp = 0x38,
	BSSM_GRASS_NOALPHA_1POINT_LVSPShp = 0x39,
	BSSM_GRASS_NOALPHA_1POINT_LFB = 0x3A,
	BSSM_GRASS_NOALPHA_1POINT_LFSB = 0x3B,
	BSSM_GRASS_NOALPHA_1POINT_LFPB = 0x3C,
	BSSM_GRASS_NOALPHA_1POINT_LFSPB = 0x3D,
	BSSM_LEAVES_AD = 0x3E,
	BSSM_LEAVES_ADPt = 0x3F,
	BSSM_FRONDS = 0x40,
	BSSM_PARTICLE_PREPASS = 0x41,
	BSSM_NOLIGHTING = 0x42,
	BSSM_NOLIGHTING_VC = 0x43,
	BSSM_NOLIGHTING_TexVC = 0x44,
	BSSM_NOLIGHTING_MAT = 0x45,
	BSSM_NOLIGHTING_S = 0x46,
	BSSM_NOLIGHTING_VC_S = 0x47,
	BSSM_NOLIGHTING_TexVC_S = 0x48,
	BSSM_NOLIGHTING_MAT_S = 0x49,
	BSSM_NOLIGHTING_FALLOFF = 0x4A,
	BSSM_NOLIGHTING_VC_FALLOFF = 0x4B,
	BSSM_NOLIGHTING_TexVC_FALLOFF = 0x4C,
	BSSM_NOLIGHTING_MAT_FALLOFF = 0x4D,
	BSSM_NOLIGHTING_FALLOFF_S = 0x4E,
	BSSM_NOLIGHTING_VC_FALLOFF_S = 0x4F,
	BSSM_NOLIGHTING_TexVC_FALLOFF_S = 0x50,
	BSSM_NOLIGHTING_MAT_FALLOFF_S = 0x51,
	BSSM_NOLIGHTING_PSYS = 0x52,
	BSSM_NOLIGHTING_PSYS_SUBTEX_OFFSET = 0x53,
	BSSM_NOLIGHTING_PSYS_PREMULT_ALPHA = 0x54,
	BSSM_NOLIGHTING_PSYS_SUBTEX_OFFSET_PREMULT_ALPHA = 0x55,
	BSSM_NOLIGHTING_STRIP_PSYS = 0x56,
	BSSM_NOLIGHTING_STRIP_PSYS_SUBTEX = 0x57,
	BSSM_AMBIENT = 0x58,
	BSSM_AMBIENT_G = 0x59,
	BSSM_AMBIENT_At = 0x5A,
	BSSM_AMBIENT_GAt = 0x5B,
	BSSM_AMBIENT_S = 0x5C,
	BSSM_AMBIENT_SG = 0x5D,
	BSSM_AMBIENT_SAt = 0x5E,
	BSSM_AMBIENT_SGAt = 0x5F,
	BSSM_AMBIENT_Sb = 0x60,
	BSSM_AMBIENT_Mn = 0x61,
	BSSM_AMBIENT_F = 0x62,
	BSSM_AMBIENT_FAt = 0x63,
	BSSM_AMBDIFFTEX = 0x64,
	BSSM_AMBDIFFTEX_Vc = 0x65,
	BSSM_AMBDIFFTEX_G = 0x66,
	BSSM_AMBDIFFTEX_GVc = 0x67,
	BSSM_AMBDIFFTEX_Fg = 0x68,
	BSSM_AMBDIFFTEX_A = 0x69,
	BSSM_AMBDIFFTEX_AVc = 0x6A,
	BSSM_AMBDIFFTEX_GA = 0x6B,
	BSSM_AMBDIFFTEX_GAVc = 0x6C,
	BSSM_AMBDIFFTEX_FgA = 0x6D,
	BSSM_AMBDIFFTEX_GFgA = 0x6E,
	BSSM_AMBDIFFTEX_S = 0x6F,
	BSSM_AMBDIFFTEX_SVc = 0x70,
	BSSM_AMBDIFFTEX_SG = 0x71,
	BSSM_AMBDIFFTEX_SGVc = 0x72,
	BSSM_AMBDIFFTEX_SFg = 0x73,
	BSSM_AMBDIFFTEX_SA = 0x74,
	BSSM_AMBDIFFTEX_SAVc = 0x75,
	BSSM_AMBDIFFTEX_SGA = 0x76,
	BSSM_AMBDIFFTEX_SGAVc = 0x77,
	BSSM_AMBDIFFTEX_SFgA = 0x78,
	BSSM_AMBDIFFTEX_SGFgA = 0x79,
	BSSM_AMBDIFFTEX_Sb = 0x7A,
	BSSM_AMBDIFFTEX_SbF = 0x7B,
	BSSM_AMBDIFFDIRANDPT = 0x7C,
	BSSM_AMBDIFFDIRANDPT_S = 0x7D,
	BSSM_AMBDIFFDIRANDPT_Sb = 0x7E,
	BSSM_AMBDIFFTEX_F = 0x7F,
	BSSM_AMBDIFFTEX_FVc = 0x80,
	BSSM_AMBDIFFTEX_FG = 0x81,
	BSSM_AMBDIFFTEX_FGVc = 0x82,
	BSSM_AMBDIFFTEX_FA = 0x83,
	BSSM_AMBDIFFTEX_FAVc = 0x84,
	BSSM_AMBDIFFTEX_FFg = 0x85,
	BSSM_AMBDIFFTEX_FGA = 0x86,
	BSSM_AMBDIFFTEX_FGAVc = 0x87,
	BSSM_AMBDIFFTEX_FGFgA = 0x88,
	BSSM_AMBDIFFTEX_SF = 0x89,
	BSSM_AMBDIFFTEX_SFVc = 0x8A,
	BSSM_AMBDIFFTEX_SFG = 0x8B,
	BSSM_AMBDIFFTEX_SFGVc = 0x8C,
	BSSM_AMBDIFFTEX_SFA = 0x8D,
	BSSM_AMBDIFFTEX_SFAVc = 0x8E,
	BSSM_AMBDIFFTEX_SFFg = 0x8F,
	BSSM_AMBDIFFTEX_SFGA = 0x90,
	BSSM_AMBDIFFTEX_SFGAVc = 0x91,
	BSSM_AMBDIFFTEX_SFGFgA = 0x92,
	BSSM_LANDAD = 0x93,
	BSSM_LANDAD_Shp = 0x94,
	BSSM_ADTS = 0x95,
	BSSM_ADTS_G = 0x96,
	BSSM_ADTS_H = 0x97,
	BSSM_ADTS_Fg = 0x98,
	BSSM_ADTS_Px = 0x99,
	BSSM_ADTS_GPx = 0x9A,
	BSSM_ADTS_S = 0x9B,
	BSSM_ADTS_SG = 0x9C,
	BSSM_ADTS_SH = 0x9D,
	BSSM_ADTS_SFg = 0x9E,
	BSSM_ADTS_Sb = 0x9F,
	BSSM_ADTS_Shp = 0xA0,
	BSSM_ADTS_GShp = 0xA1,
	BSSM_ADTS_HShp = 0xA2,
	BSSM_ADTS_FgShp = 0xA3,
	BSSM_ADTS_PxShp = 0xA4,
	BSSM_ADTS_GPxShp = 0xA5,
	BSSM_ADTS_SShp = 0xA6,
	BSSM_ADTS_SGShp = 0xA7,
	BSSM_ADTS_SHShp = 0xA8,
	BSSM_ADTS_SFgShp = 0xA9,
	BSSM_ADTS_SbShp = 0xAA,
	BSSM_ADTS2 = 0xAB,
	BSSM_ADTS2_G = 0xAC,
	BSSM_ADTS2_H = 0xAD,
	BSSM_ADTS2_Fg = 0xAE,
	BSSM_ADTS2_Px = 0xAF,
	BSSM_ADTS2_GPx = 0xB0,
	BSSM_ADTS2_S = 0xB1,
	BSSM_ADTS2_SG = 0xB2,
	BSSM_ADTS2_SH = 0xB3,
	BSSM_ADTS2_SFg = 0xB4,
	BSSM_ADTS2_Sb = 0xB5,
	BSSM_ADTS2_Shp = 0xB6,
	BSSM_ADTS2_GShp = 0xB7,
	BSSM_ADTS2_HShp = 0xB8,
	BSSM_ADTS2_FgShp = 0xB9,
	BSSM_ADTS2_PxShp = 0xBA,
	BSSM_ADTS2_GPxShp = 0xBB,
	BSSM_ADTS2_SShp = 0xBC,
	BSSM_ADTS2_SGShp = 0xBD,
	BSSM_ADTS2_SHShp = 0xBE,
	BSSM_ADTS2_SFgShp = 0xBF,
	BSSM_ADTS2_SbShp = 0xC0,
	BSSM_ADTS_ONELIGHT = 0xC1,
	BSSM_ADTS_DIRANDPT = 0xC2,
	BSSM_ADT10 = 0xC3,
	BSSM_ADT10_G = 0xC4,
	BSSM_ADT10_Fg = 0xC5,
	BSSM_ADT10_S = 0xC6,
	BSSM_ADT10_SG = 0xC7,
	BSSM_ADT10_SFg = 0xC8,
	BSSM_ADT4 = 0xC9,
	BSSM_ADT4_Opt = 0xCA,
	BSSM_ADT4_G = 0xCB,
	BSSM_ADT4_Fg = 0xCC,
	BSSM_ADT4_S = 0xCD,
	BSSM_ADT4_SG = 0xCE,
	BSSM_ADT4_SFg = 0xCF,
	BSSM_ADTS10 = 0xD0,
	BSSM_ADTS10_Opt = 0xD1,
	BSSM_ADTS10_G = 0xD2,
	BSSM_ADTS10_Fg = 0xD3,
	BSSM_ADTS10_S = 0xD4,
	BSSM_ADTS10_SG = 0xD5,
	BSSM_ADTS10_SFg = 0xD6,
	BSSM_AD2 = 0xD7,
	BSSM_AD2_G = 0xD8,
	BSSM_AD2_Px = 0xD9,
	BSSM_AD2_GPx = 0xDA,
	BSSM_AD2_Fg = 0xDB,
	BSSM_AD2_S = 0xDC,
	BSSM_AD2_SG = 0xDD,
	BSSM_AD2_SFg = 0xDE,
	BSSM_AD2_Sb = 0xDF,
	BSSM_AD2_Shp = 0xE0,
	BSSM_AD2_GShp = 0xE1,
	BSSM_AD2_PxShp = 0xE2,
	BSSM_AD2_GPxShp = 0xE3,
	BSSM_AD2_FgShp = 0xE4,
	BSSM_AD2_SShp = 0xE5,
	BSSM_AD2_SGShp = 0xE6,
	BSSM_AD2_SFgShp = 0xE7,
	BSSM_AD2_SbShp = 0xE8,
	BSSM_AD3 = 0xE9,
	BSSM_AD3_G = 0xEA,
	BSSM_AD3_Px = 0xEB,
	BSSM_AD3_GPx = 0xEC,
	BSSM_AD3_Fg = 0xED,
	BSSM_AD3_S = 0xEE,
	BSSM_AD3_SG = 0xEF,
	BSSM_AD3_SFg = 0xF0,
	BSSM_AD3_Sb = 0xF1,
	BSSM_AD3_Shp = 0xF2,
	BSSM_AD3_GShp = 0xF3,
	BSSM_AD3_PxShp = 0xF4,
	BSSM_AD3_GPxShp = 0xF5,
	BSSM_AD3_FgShp = 0xF6,
	BSSM_AD3_SShp = 0xF7,
	BSSM_AD3_SGShp = 0xF8,
	BSSM_AD3_SFgShp = 0xF9,
	BSSM_AD3_SbShp = 0xFA,
	BSSM_ADT = 0xFB,
	BSSM_ADT_Opt = 0xFC,
	BSSM_ADT_OptLOD = 0xFD,
	BSSM_ADT_Mn = 0xFE,
	BSSM_ADT_G = 0xFF,
	BSSM_ADT_Fg = 0x100,
	BSSM_ADT_Px = 0x101,
	BSSM_ADT_PxOpt = 0x102,
	BSSM_ADT_GPx = 0x103,
	BSSM_ADT_H = 0x104,
	BSSM_ADT_S = 0x105,
	BSSM_ADT_SG = 0x106,
	BSSM_ADT_SFg = 0x107,
	BSSM_ADT_Sb = 0x108,
	BSSM_ADT_SH = 0x109,
	BSSM_ADT_Shp = 0x10A,
	BSSM_ADT_MnShp = 0x10B,
	BSSM_ADT_GShp = 0x10C,
	BSSM_ADT_FgShp = 0x10D,
	BSSM_ADT_PxShp = 0x10E,
	BSSM_ADT_GPxShp = 0x10F,
	BSSM_ADT_HShp = 0x110,
	BSSM_ADT_SShp = 0x111,
	BSSM_ADT_SGShp = 0x112,
	BSSM_ADT_SFgShp = 0x113,
	BSSM_ADT_SbShp = 0x114,
	BSSM_ADT_SHShp = 0x115,
	BSSM_ADT2 = 0x116,
	BSSM_ADT2_G = 0x117,
	BSSM_ADT2_Fg = 0x118,
	BSSM_ADT2_Px = 0x119,
	BSSM_ADT2_GPx = 0x11A,
	BSSM_ADT2_H = 0x11B,
	BSSM_ADT2_S = 0x11C,
	BSSM_ADT2_SG = 0x11D,
	BSSM_ADT2_SFg = 0x11E,
	BSSM_ADT2_Sb = 0x11F,
	BSSM_ADT2_SH = 0x120,
	BSSM_ADT2_Shp = 0x121,
	BSSM_ADT2_GShp = 0x122,
	BSSM_ADT2_FgShp = 0x123,
	BSSM_ADT2_PxShp = 0x124,
	BSSM_ADT2_GPxShp = 0x125,
	BSSM_ADT2_HShp = 0x126,
	BSSM_ADT2_SShp = 0x127,
	BSSM_ADT2_SGShp = 0x128,
	BSSM_ADT2_SFgShp = 0x129,
	BSSM_ADT2_SbShp = 0x12A,
	BSSM_ADT2_SHShp = 0x12B,
	BSSM_DIFFUSEDIR = 0x12C,
	BSSM_DIFFUSEPT = 0x12D,
	BSSM_DIFFUSEDIR_S = 0x12E,
	BSSM_DIFFUSEPT_S = 0x12F,
	BSSM_DIFFUSEDIR_Sb = 0x130,
	BSSM_DIFFUSEPT_Sb = 0x131,
	BSSM_DIFFUSEPT2 = 0x132,
	BSSM_DIFFUSEPT2_Fg = 0x133,
	BSSM_DIFFUSEPT2_Px = 0x134,
	BSSM_DIFFUSEPT2_S = 0x135,
	BSSM_DIFFUSEPT2_SFg = 0x136,
	BSSM_DIFFUSEPT2_Sb = 0x137,
	BSSM_DIFFUSEPT2_Shp = 0x138,
	BSSM_DIFFUSEPT2_FgShp = 0x139,
	BSSM_DIFFUSEPT2_PxShp = 0x13A,
	BSSM_DIFFUSEPT2_SShp = 0x13B,
	BSSM_DIFFUSEPT2_SFgShp = 0x13C,
	BSSM_DIFFUSEPT2_SbShp = 0x13D,
	BSSM_DIFFUSEPT3 = 0x13E,
	BSSM_DIFFUSEPT3_Fg = 0x13F,
	BSSM_DIFFUSEPT3_Px = 0x140,
	BSSM_DIFFUSEPT3_S = 0x141,
	BSSM_DIFFUSEPT3_SFg = 0x142,
	BSSM_DIFFUSEPT3_Sb = 0x143,
	BSSM_DIFFUSEPT3_Shp = 0x144,
	BSSM_DIFFUSEPT3_FgShp = 0x145,
	BSSM_DIFFUSEPT3_PxShp = 0x146,
	BSSM_DIFFUSEPT3_SShp = 0x147,
	BSSM_DIFFUSEPT3_SFgShp = 0x148,
	BSSM_DIFFUSEPT3_SbShp = 0x149,
	BSSM_TEXTURE = 0x14A,
	BSSM_TEXTURE_Fg = 0x14B,
	BSSM_TEXTURE_H = 0x14C,
	BSSM_TEXTURE_S = 0x14D,
	BSSM_TEXTURE_Vc = 0x14E,
	BSSM_TEXTURE_SVc = 0x14F,
	BSSM_TEXTURE_SFg = 0x150,
	BSSM_TEXTURE_Sb = 0x151,
	BSSM_TEXTURE_SH = 0x152,
	BSSM_TEXTURE_Px = 0x153,
	BSSM_SPECULARDIR = 0x154,
	BSSM_SPECULARPT = 0x155,
	BSSM_SPECULARDIR_S = 0x156,
	BSSM_SPECULARPT_S = 0x157,
	BSSM_2x_SPECULARDIR = 0x15A,
	BSSM_2x_SPECULARDIR_H = 0x15B,
	BSSM_2x_SPECULARDIR_Px = 0x15C,
	BSSM_2x_SPECULARDIR_S = 0x15D,
	BSSM_2x_SPECULARDIR_SH = 0x15E,
	BSSM_2x_SPECULARDIR_Sb = 0x15F,
	BSSM_2x_SPECULARDIR_Shp = 0x160,
	BSSM_2x_SPECULARDIR_HShp = 0x161,
	BSSM_2x_SPECULARDIR_PxShp = 0x162,
	BSSM_2x_SPECULARDIR_SShp = 0x163,
	BSSM_2x_SPECULARDIR_SHShp = 0x164,
	BSSM_2x_SPECULARDIR_SbShp = 0x165,
	BSSM_2x_SPECULARPT = 0x166,
	BSSM_2x_SPECULARPT_H = 0x167,
	BSSM_2x_SPECULARPT_Px = 0x168,
	BSSM_2x_SPECULARPT_S = 0x169,
	BSSM_2x_SPECULARPT_SH = 0x16A,
	BSSM_2x_SPECULARPT_Sb = 0x16B,
	BSSM_2x_SPECULARPT2 = 0x16C,
	BSSM_2x_SPECULARPT2_H = 0x16D,
	BSSM_2x_SPECULARPT2_Px = 0x16E,
	BSSM_2x_SPECULARPT2_S = 0x16F,
	BSSM_2x_SPECULARPT2_SH = 0x170,
	BSSM_2x_SPECULARPT2_Sb = 0x171,
	BSSM_2x_SPECULARPT3 = 0x172,
	BSSM_2x_SPECULARPT3_H = 0x173,
	BSSM_2x_SPECULARPT3_Px = 0x174,
	BSSM_2x_SPECULARPT3_S = 0x175,
	BSSM_2x_SPECULARPT3_SH = 0x176,
	BSSM_2x_SPECULARPT3_Sb = 0x177,
	BSSM_3XOCCLUSION = 0x178,
	BSSM_3XLIGHTING = 0x17F,
	BSSM_3XLIGHTING_S = 0x180,
	BSSM_3XLIGHTING_H = 0x181,
	BSSM_3XLIGHTING_Px = 0x182,
	BSSM_3XLIGHTING_Pxo = 0x183,
	BSSM_3XLIGHTING_Fg = 0x184,
	BSSM_3XLIGHTING_SFg = 0x185,
	BSSM_3XLIGHTING_G = 0x186,
	BSSM_3XLIGHTING_SG = 0x187,
	BSSM_3XLIGHTING_Vc = 0x188,
	BSSM_3XLIGHTING_VcS = 0x189,
	BSSM_3XLIGHTING_VcPx = 0x18A,
	BSSM_3XLIGHTING_VcPxo = 0x18B,
	BSSM_3XLIGHTING_VcG = 0x18C,
	BSSM_3XLIGHTING_VcSG = 0x18D,
	BSSM_3XLIGHTING_Spc = 0x18E,
	BSSM_3XLIGHTING_SSpc = 0x18F,
	BSSM_3XLIGHTING_HSpc = 0x190,
	BSSM_3XLIGHTING_PxSpc = 0x191,
	BSSM_3XLIGHTING_PxoSpc = 0x192,
	BSSM_3XLIGHTING_FgSpc = 0x193,
	BSSM_3XLIGHTING_SFgSpc = 0x194,
	BSSM_3XLIGHTING_GSpc = 0x195,
	BSSM_3XLIGHTING_SGSpc = 0x196,
	BSSM_3XLIGHTING_VcSpc = 0x197,
	BSSM_3XLIGHTING_VcSSpc = 0x198,
	BSSM_3XLIGHTING_VcPxSpc = 0x199,
	BSSM_3XLIGHTING_VcPxoSpc = 0x19A,
	BSSM_3XLIGHTING_VcGSpc = 0x19B,
	BSSM_3XLIGHTING_VcSGSpc = 0x19C,
	BSSM_3XLIGHTING_Shp = 0x19D,
	BSSM_3XLIGHTING_SShp = 0x19E,
	BSSM_3XLIGHTING_HShp = 0x19F,
	BSSM_3XLIGHTING_PxShp = 0x1A0,
	BSSM_3XLIGHTING_PxoShp = 0x1A1,
	BSSM_3XLIGHTING_FgShp = 0x1A2,
	BSSM_3XLIGHTING_SFgShp = 0x1A3,
	BSSM_3XLIGHTING_GShp = 0x1A4,
	BSSM_3XLIGHTING_SGShp = 0x1A5,
	BSSM_3XLIGHTING_VcShp = 0x1A6,
	BSSM_3XLIGHTING_VcSShp = 0x1A7,
	BSSM_3XLIGHTING_VcPxShp = 0x1A8,
	BSSM_3XLIGHTING_VcPxoShp = 0x1A9,
	BSSM_3XLIGHTING_VcGShp = 0x1AA,
	BSSM_3XLIGHTING_VcSGShp = 0x1AB,
	BSSM_3XLIGHTING_ShpSpc = 0x1AC,
	BSSM_3XLIGHTING_SShpSpc = 0x1AD,
	BSSM_3XLIGHTING_HShpSpc = 0x1AE,
	BSSM_3XLIGHTING_PxShpSpc = 0x1AF,
	BSSM_3XLIGHTING_PxoShpSpc = 0x1B0,
	BSSM_3XLIGHTING_FgShpSpc = 0x1B1,
	BSSM_3XLIGHTING_SFgShpSpc = 0x1B2,
	BSSM_3XLIGHTING_GShpSpc = 0x1B3,
	BSSM_3XLIGHTING_SGShpSpc = 0x1B4,
	BSSM_3XLIGHTING_VcShpSpc = 0x1B5,
	BSSM_3XLIGHTING_VcSShpSpc = 0x1B6,
	BSSM_3XLIGHTING_VcPxShpSpc = 0x1B7,
	BSSM_3XLIGHTING_VcPxoShpSpc = 0x1B8,
	BSSM_3XLIGHTING_VcGShpSpc = 0x1B9,
	BSSM_3XLIGHTING_VcSGShpSpc = 0x1BA,
	BSSM_3XSILHOUETTE = 0x1BB,
	BSSM_3XSILHOUETTE_Vc = 0x1BC,
	BSSM_3XENVMAP = 0x1BD,
	BSSM_3XENVMAP_W = 0x1BE,
	BSSM_3XENVMAP_Vc = 0x1BF,
	BSSM_3XENVMAP_WVc = 0x1C0,
	BSSM_3XENVMAP_S = 0x1C1,
	BSSM_3XENVMAP_SVc = 0x1C2,
	BSSM_3XENVMAP_EYE = 0x1C3,
	BSSM_3XLIGHTING_SIMPLESHADOW = 0x1C4,
	BSSM_3XLIGHTING_SIMPLESHADOW_Vc = 0x1C5,
	BSSM_3XLIGHTING_SIMPLESHADOW_S = 0x1C6,
	BSSM_3XLIGHTING_SIMPLESHADOW_VcS = 0x1C7,
	BSSM_3XDEPTHMAP = 0x1C8,
	BSSM_3XDEPTHMAP_Vc = 0x1C9,
	BSSM_3XDEPTHMAP_S = 0x1CA,
	BSSM_3XDEPTHMAP_VcS = 0x1CB,
	BSSM_3XRENDERNORMALS = 0x1CC,
	BSSM_3XRENDERNORMALS_Vc = 0x1CD,
	BSSM_3XRENDERNORMALS_S = 0x1CE,
	BSSM_3XRENDERNORMALS_VcS = 0x1CF,
	BSSM_3XRENDERNORMALS_FIRE = 0x1D0,
	BSSM_3XRENDERNORMALS_CLEAR = 0x1D1,
	BSSM_3XRENDERNORMALS_CLEAR_S = 0x1D2,
	BSSM_3XCONSTALPHA = 0x1D3,
	BSSM_3XCONSTALPHA_Vc = 0x1D4,
	BSSM_3XCONSTALPHA_S = 0x1D5,
	BSSM_3XCONSTALPHA_SVc = 0x1D6,
	BSSM_3XDEPTH = 0x1D7,
	BSSM_3XDEPTH_Vc = 0x1D8,
	BSSM_3XDEPTH_S = 0x1D9,
	BSSM_3XDEPTH_SVc = 0x1DA,
	BSSM_3XVELOCITY = 0x1DB,
	BSSM_3XVELOCITY_S = 0x1DC,
	BSSM_3XLOCALMAP = 0x1DD,
	BSSM_3XLOCALMAP_S = 0x1DE,
	BSSM_3XLOCALMAP_CLEAR = 0x1DF,
	BSSM_3XTEXEFFECT = 0x1E0,
	BSSM_3XTEXEFFECT_S = 0x1E1,
	BSSM_RENDERNORMALS = 0x1E2,
	BSSM_RENDERNORMALS_S = 0x1E3,
	BSSM_RENDERNORMALS_FIRE = 0x1E4,
	BSSM_RENDERNORMALS_CLEAR = 0x1E5,
	BSSM_RENDERNORMALS_CLEAR_S = 0x1E6,
	BSSM_DEPTH = 0x1E7,
	BSSM_DEPTH_S = 0x1E8,
	BSSM_DEPTH_Mn = 0x1E9,
	BSSM_VELOCITY = 0x1EA,
	BSSM_VELOCITY_S = 0x1EB,
	BSSM_LOCALMAP = 0x1EC,
	BSSM_LOCALMAP_S = 0x1ED,
	BSSM_LOCALMAP_CLEAR = 0x1EE,
	BSSM_LANDDIFF = 0x1EF,
	BSSM_LAND2xDIFF = 0x1F0,
	BSSM_LAND_G = 0x1F1,
	BSSM_LANDAD_A = 0x1F2,
	BSSM_LANDAD_AShp = 0x1F3,
	BSSM_LAND_GA = 0x1F4,
	BSSM_LANDDIFF_A = 0x1F5,
	BSSM_LAND2xDIFF_A = 0x1F6,
	BSSM_LAND1O = 0x1F7,
	BSSM_LAND1O_Shp = 0x1F8,
	BSSM_LAND1O_Simple = 0x1F9,
	BSSM_LAND1O_SimpleShp = 0x1FA,
	BSSM_LAND1O4 = 0x1FB,
	BSSM_LAND1O4_Shp = 0x1FC,
	BSSM_LAND1O4_Simple = 0x1FD,
	BSSM_LAND1O4_SimpleShp = 0x1FE,
	BSSM_LAND2O = 0x1FF,
	BSSM_LAND2O_Shp = 0x200,
	BSSM_LAND2O_Simple = 0x201,
	BSSM_LAND2O_SimpleShp = 0x202,
	BSSM_LAND2O4 = 0x203,
	BSSM_LAND2O4_Shp = 0x204,
	BSSM_LAND2O4_Simple = 0x205,
	BSSM_LAND2O4_SimpleShp = 0x206,
	BSSM_LAND3O = 0x207,
	BSSM_LAND3O_Shp = 0x208,
	BSSM_LAND3O_Simple = 0x209,
	BSSM_LAND3O_SimpleShp = 0x20A,
	BSSM_LAND3O4 = 0x20B,
	BSSM_LAND3O4_Shp = 0x20C,
	BSSM_LAND3O4_Simple = 0x20D,
	BSSM_LAND3O4_SimpleShp = 0x20E,
	BSSM_LAND4O = 0x20F,
	BSSM_LAND4O_Shp = 0x210,
	BSSM_LAND4O_Simple = 0x211,
	BSSM_LAND4O_SimpleShp = 0x212,
	BSSM_LAND4O4 = 0x213,
	BSSM_LAND4O4_Shp = 0x214,
	BSSM_LAND4O4_Simple = 0x215,
	BSSM_LAND4O4_SimpleShp = 0x216,
	BSSM_LAND5O = 0x217,
	BSSM_LAND5O_Shp = 0x218,
	BSSM_LAND5O_Simple = 0x219,
	BSSM_LAND5O_SimpleShp = 0x21A,
	BSSM_LAND5O4 = 0x21B,
	BSSM_LAND5O4_Shp = 0x21C,
	BSSM_LAND5O4_Simple = 0x21D,
	BSSM_LAND5O4_SimpleShp = 0x21E,
	BSSM_LAND6O = 0x21F,
	BSSM_LAND6O_Shp = 0x220,
	BSSM_LAND6O_Simple = 0x221,
	BSSM_LAND6O_SimpleShp = 0x222,
	BSSM_LAND6O4 = 0x223,
	BSSM_LAND6O4_Shp = 0x224,
	BSSM_LAND6O4_Simple = 0x225,
	BSSM_LAND6O4_SimpleShp = 0x226,
	BSSM_LAND7O = 0x227,
	BSSM_LAND7O_Shp = 0x228,
	BSSM_LAND7O_Simple = 0x229,
	BSSM_LAND7O_SimpleShp = 0x22A,
	BSSM_LAND7O4 = 0x22B,
	BSSM_LAND7O4_Shp = 0x22C,
	BSSM_LAND7O4_Simple = 0x22D,
	BSSM_LAND7O4_SimpleShp = 0x22E,
	BSSM_LANDO_Am = 0x22F,
	BSSM_LANDLO_A = 0x230,
	BSSM_2x_SIMPLESHADOW = 0x231,
	BSSM_2x_SIMPLESHADOW_S = 0x232,
	BSSM_2x_SIMPLESHADOW_LAND = 0x233,
	BSSM_2x_SIMPLESHADOW_Sb = 0x234,
	BSSM_ADT_Sbb = 0x235,
	BSSM_DISTANT_TREE_BASE = 0x236,
	BSSM_UNK_237 = 0x237,
	BSSM_SKYBASEPRE = 0x238,
	BSSM_SKY = 0x239,
	BSSM_SKY_MOON_STARS_MASK = 0x23A,
	BSSM_SKY_STARS = 0x23B,
	BSSM_SKY_TEXTURE = 0x23C,
	BSSM_SKY_CLOUDS = 0x23D,
	BSSM_SKY_CLOUDS_UNDERWATER = 0x23E,
	BSSM_WATER_LOD = 0x23F,
	BSSM_WATER_LOD_NO_REFLECTIONS = 0x240,
	BSSM_PARTICLE = 0x241,
	BSSM_BOLT = 0x242,
	BSSM_BEAM = 0x243,
	BSSM_ENVMAP = 0x244,
	BSSM_ENVMAP_Vc = 0x245,
	BSSM_ENVMAP_S = 0x246,
	BSSM_ENVMAP_SVc = 0x247,
	BSSM_2x_ENVMAP = 0x248,
	BSSM_2x_ENVMAP_S = 0x249,
	BSSM_2x_ENVMAP_W = 0x24A,
	BSSM_2x_ENVMAP_EYE = 0x24B,
	BSSM_GEOMDECAL = 0x24C,
	BSSM_GEOMDECAL_S = 0x24D,
	BSSM_TEXEFFECT = 0x24E,
	BSSM_TEXEFFECT_S = 0x24F,
	BSSM_2x_TEXEFFECT = 0x250,
	BSSM_2x_TEXEFFECT_S = 0x251,
	BSSM_FOG = 0x252,
	BSSM_FOG_A = 0x253,
	BSSM_FOG_S = 0x254,
	BSSM_FOG_SA = 0x255,
	BSSM_FOG_Sb = 0x256,
	BSSM_VFOG_1 = 0x259,
	BSSM_VFOG_2 = 0x25A,
	BSSM_VFOG_3 = 0x25B,
	BSSM_GRASS_DIRONLY_LF = 0x25C,
	BSSM_GRASS_DIRONLY_LV = 0x25D,
	BSSM_GRASS_DIRONLY_LFS = 0x25E,
	BSSM_GRASS_DIRONLY_LVS = 0x25F,
	BSSM_GRASS_DIRONLY_LFP = 0x260,
	BSSM_GRASS_DIRONLY_LVP = 0x261,
	BSSM_GRASS_DIRONLY_LFSP = 0x262,
	BSSM_GRASS_DIRONLY_LVSP = 0x263,
	BSSM_GRASS_DIRONLY_LFShp = 0x264,
	BSSM_GRASS_DIRONLY_LVShp = 0x265,
	BSSM_GRASS_DIRONLY_LFSShp = 0x266,
	BSSM_GRASS_DIRONLY_LVSShp = 0x267,
	BSSM_GRASS_DIRONLY_LFPShp = 0x268,
	BSSM_GRASS_DIRONLY_LVPShp = 0x269,
	BSSM_GRASS_DIRONLY_LFSPShp = 0x26A,
	BSSM_GRASS_DIRONLY_LVSPShp = 0x26B,
	BSSM_GRASS_DIRONLY_LFB = 0x26C,
	BSSM_GRASS_DIRONLY_LFSB = 0x26D,
	BSSM_GRASS_DIRONLY_LFPB = 0x26E,
	BSSM_GRASS_DIRONLY_LFSPB = 0x26F,
	BSSM_GRASS_1POINT_LF = 0x270,
	BSSM_GRASS_1POINT_LV = 0x271,
	BSSM_GRASS_1POINT_LFS = 0x272,
	BSSM_GRASS_1POINT_LVS = 0x273,
	BSSM_GRASS_1POINT_LFP = 0x274,
	BSSM_GRASS_1POINT_LVP = 0x275,
	BSSM_GRASS_1POINT_LFSP = 0x276,
	BSSM_GRASS_1POINT_LVSP = 0x277,
	BSSM_GRASS_1POINT_LFShp = 0x278,
	BSSM_GRASS_1POINT_LVShp = 0x279,
	BSSM_GRASS_1POINT_LFSShp = 0x27A,
	BSSM_GRASS_1POINT_LVSShp = 0x27B,
	BSSM_GRASS_1POINT_LFPShp = 0x27C,
	BSSM_GRASS_1POINT_LVPShp = 0x27D,
	BSSM_GRASS_1POINT_LFSPShp = 0x27E,
	BSSM_GRASS_1POINT_LVSPShp = 0x27F,
	BSSM_GRASS_1POINT_LFB = 0x280,
	BSSM_GRASS_1POINT_LFSB = 0x281,
	BSSM_GRASS_1POINT_LFPB = 0x282,
	BSSM_GRASS_1POINT_LFSPB = 0x283,
	BSSM_GRASS_SHADOW_L = 0x284,
	BSSM_GRASS_SHADOW_LS = 0x285,
	BSSM_GRASS_SHADOW_LB = 0x286,
	BSSM_GRASS_SHADOW_LSB = 0x287,
	BSSM_WATER_STENCIL = 0x288,
	BSSM_WATER_STENCIL_Vc = 0x289,
	BSSM_WATER_WADING = 0x28A,
	BSSM_WATER_WADING_NO_REFLECTIONS_NO_REFRACTIONS = 0x28B,
	BSSM_WATER_WADING_NO_REFRACTIONS = 0x28C,
	BSSM_WATER_WADING_NO_REFLECTIONS = 0x28D,
	BSSM_WATER_WADING_NO_DEPTH = 0x28E,
	BSSM_WATER_WADING_NO_DEPTH_NO_REFLECTIONS_NO_REFRACTIONS = 0x28F,
	BSSM_WATER_WADING_NO_DEPTH_NO_REFRACTIONS = 0x290,
	BSSM_WATER_WADING_NO_DEPTH_NO_REFLECTIONS = 0x291,
	BSSM_WATER_INTERIOR_WADING = 0x292,
	BSSM_WATER_INTERIOR_WADING_NO_REFLECTIONS_NO_REFRACTIONS = 0x293,
	BSSM_WATER_INTERIOR_WADING_NO_REFRACTIONS = 0x294,
	BSSM_WATER_INTERIOR_WADING_NO_REFLECTIONS = 0x295,
	BSSM_WATER_INTERIOR_WADING_NO_DEPTH = 0x296,
	BSSM_WATER_INTERIOR_WADING_NO_DEPTH_NO_REFLECTIONS_NO_REFRACTIONS = 0x297,
	BSSM_WATER_INTERIOR_WADING_NO_DEPTH_NO_REFRACTIONS = 0x298,
	BSSM_WATER_INTERIOR_WADING_NO_DEPTH_NO_REFLECTIONS = 0x299,
	BSSM_WATER_WADING_SPECULAR_LIGHTING = 0x29A,
	BSSM_WATER_WADING_Vc = 0x29B,
	BSSM_WATER_WADING_NO_REFLECTIONS_Vc = 0x29C,
	BSSM_WATER_WADING_NO_REFRACTIONS_Vc = 0x29D,
	BSSM_WATER_WADING_NO_REFLECTIONS_NO_REFRACTIONS_Vc = 0x29E,
	BSSM_WATER_WADING_NO_DEPTH_Vc = 0x29F,
	BSSM_WATER_WADING_NO_DEPTH_NO_REFLECTIONS_Vc = 0x2A0,
	BSSM_WATER_WADING_NO_DEPTH_NO_REFRACTIONS_Vc = 0x2A1,
	BSSM_WATER_WADING_NO_DEPTH_NO_REFLECTIONS_NO_REFRACTIONS_Vc = 0x2A2,
	BSSM_WATER_INTERIOR_WADING_Vc = 0x2A3,
	BSSM_WATER_INTERIOR_WADING_NO_REFLECTIONS_Vc = 0x2A4,
	BSSM_WATER_INTERIOR_WADING_NO_REFRACTIONS_Vc = 0x2A5,
	BSSM_WATER_INTERIOR_WADING_NO_REFLECTIONS_NO_REFRACTIONS_Vc = 0x2A6,
	BSSM_WATER_INTERIOR_WADING_NO_DEPTH_Vc = 0x2A7,
	BSSM_WATER_INTERIOR_WADING_NO_DEPTH_NO_REFLECTIONS_Vc = 0x2A8,
	BSSM_WATER_INTERIOR_WADING_NO_DEPTH_NO_REFRACTIONS_Vc = 0x2A9,
	BSSM_WATER_INTERIOR_WADING_NO_DEPTH_NO_REFLECTIONS_NO_REFRACTIONS_Vc = 0x2AA,
	BSSM_WATER_WADING_SPECULAR_LIGHTING_Vc = 0x2AB,
	BSSM_WATER = 0x2AC,
	BSSM_WATER_NO_REFLECTIONS_NO_REFRACTIONS = 0x2AD,
	BSSM_WATER_NO_REFRACTIONS = 0x2AE,
	BSSM_WATER_NO_REFLECTIONS = 0x2AF,
	BSSM_WATER_INTERIOR = 0x2B0,
	BSSM_WATER_INTERIOR_NO_REFLECTIONS_NO_REFRACTIONS = 0x2B1,
	BSSM_WATER_INTERIOR_NO_REFRACTIONS = 0x2B2,
	BSSM_WATER_INTERIOR_NO_REFLECTIONS = 0x2B3,
	BSSM_WATER_NODEPTH = 0x2B4,
	BSSM_WATER_NODEPTH_NO_REFLECTIONS = 0x2B5,
	BSSM_WATER_NODEPTH_NO_REFRACTIONS = 0x2B6,
	BSSM_WATER_NODEPTH_NO_REFLECTIONS_NO_REFRACTIONS = 0x2B7,
	BSSM_WATER_INTERIOR_NODEPTH = 0x2B8,
	BSSM_WATER_INTERIOR_NODEPTH_NO_REFLECTIONS_NO_REFRACTIONS = 0x2B9,
	BSSM_WATER_INTERIOR_NODEPTH_NO_REFRACTIONS = 0x2BA,
	BSSM_WATER_INTERIOR_NODEPTH_NO_REFLECTIONS = 0x2BB,
	BSSM_WATER_SPECULAR_LIGHTING = 0x2BC,
	BSSM_WATER_UNDERWATER = 0x2BD,
	BSSM_WATER_Vc = 0x2BE,
	BSSM_WATER_NO_REFLECTIONS_NO_REFRACTIONS_Vc = 0x2BF,
	BSSM_WATER_NO_REFRACTIONS_Vc = 0x2C0,
	BSSM_WATER_NO_REFLECTIONS_Vc = 0x2C1,
	BSSM_WATER_INTERIOR_Vc = 0x2C2,
	BSSM_WATER_INTERIOR_NO_REFLECTIONS_NO_REFRACTIONS_Vc = 0x2C3,
	BSSM_WATER_INTERIOR_NO_REFRACTIONS_Vc = 0x2C4,
	BSSM_WATER_INTERIOR_NO_REFLECTIONS_Vc = 0x2C5,
	BSSM_WATER_NODEPTH_Vc = 0x2C6,
	BSSM_WATER_NODEPTH_NO_REFLECTIONS_Vc = 0x2C7,
	BSSM_WATER_NODEPTH_NO_REFRACTIONS_Vc = 0x2C8,
	BSSM_WATER_NODEPTH_NO_REFLECTIONS_NO_REFRACTIONS_Vc = 0x2C9,
	BSSM_WATER_INTERIOR_NODEPTH_Vc = 0x2CA,
	BSSM_WATER_INTERIOR_NODEPTH_NO_REFLECTIONS_NO_REFRACTIONS_Vc = 0x2CB,
	BSSM_WATER_INTERIOR_NODEPTH_NO_REFRACTIONS_Vc = 0x2CC,
	BSSM_WATER_INTERIOR_NODEPTH_NO_REFLECTIONS_Vc = 0x2CD,
	BSSM_WATER_SPECULAR_LIGHTING_Vc = 0x2CE,
	BSSM_PRECIPITATION_RAIN = 0x2CF,
	BSSM_SKYBASEPOST = 0x2D0,
	BSSM_SELFILLUM_SKY = 0x2D1,
	BSSM_SKY_SUNGLARE = 0x2D2,
	BSSM_SELFILLUM_SKY_SUN = 0x2D3,
	BSSM_SELFILLUM_SKY_CLOUDS = 0x2D4,
	BSSM_SELFILLUM_SKY_SKY_QUAD = 0x2D5,
	BSSM_SELFILLUMALPHA = 0x2D6,
	BSSM_SELFILLUMALPHA_S = 0x2D7,
	BSSM_WATER_DEPTH = 0x2D8,
	BSSM_WATER_DEPTH_S = 0x2D9,
	BSSM_3XWATER_DEPTH = 0x2DA,
	BSSM_3XWATER_DEPTH_S = 0x2DB,
	BSSM_NLWATER_DEPTH = 0x2DC,
	BSSM_NLWATER_DEPTH_S = 0x2DD,
	BSSM_WATER_DEPTH_At = 0x2DE,
	BSSM_WATER_DEPTH_SAt = 0x2DF,
	BSSM_3XWATER_DEPTH_At = 0x2E0,
	BSSM_3XWATER_DEPTH_SAt = 0x2E1,
	BSSM_NLWATER_DEPTH_At = 0x2E2,
	BSSM_NLWATER_DEPTH_At_S = 0x2E3,
	BSSM_WATER_DEPTH_Vc = 0x2E4,
	BSSM_WATER_DEPTH_SVc = 0x2E5,
	BSSM_3XWATER_DEPTH_Vc = 0x2E6,
	BSSM_3XWATER_DEPTH_SVc = 0x2E7,
	BSSM_NLWATER_DEPTH_Vc = 0x2E8,
	BSSM_NLWATER_DEPTH_Vc_S = 0x2E9,
	BSSM_WATER_DEPTH_AtVc = 0x2EA,
	BSSM_WATER_DEPTH_SAtVc = 0x2EB,
	BSSM_3XWATER_DEPTH_AtVc = 0x2EC,
	BSSM_3XWATER_DEPTH_SAtVc = 0x2ED,
	BSSM_NLWATER_DEPTH_AtVc = 0x2EE,
	BSSM_NLWATER_DEPTH_AtVc_S = 0x2EF,
	BSSM_BLOOD_SPLATTER_FLARE = 0x2F0,
	BSSM_BLOOD_SPLATTER = 0x2F1,
	BSSM_SHADOWVOLUME_BACK = 0x2F2,
	BSSM_SHADOWVOLUME_FRONT = 0x2F3,
	BSSM_SHADOWVOLUME_WIRE = 0x2F4,
	BSSM_TILE = 0x2F5,
	BSSM_IMAGESPACE = 0x2F6,
	BSSM_FINAL_PASS = 0x2F7,
};

class SpeedTreeShaderLightingProperty : public BSShaderLightingProperty {
public:
	UInt32	Unk07C[3];	// 07C
};
static_assert(sizeof(SpeedTreeShaderLightingProperty) == 0x88);

class SpeedTreeLeafShaderProperty : public SpeedTreeShaderLightingProperty {
public:
	struct LeafData {
		UInt32		unk00;
		UInt32		unk04;
		float*		leafBase; // pointer to float4[48]
	};

	LeafData*	leafData;	// 088
};
static_assert(sizeof(SpeedTreeLeafShaderProperty) == 0x8C);

class BSFadeNode : public NiNode {
public:
	float			unkAC;			// AC
	float			unkB0;			// B0
	float			unkB4;			// B4
	float			FadeAlpha;		// B8
	UInt32			unkBC;			// BC
	UInt32			unkC0;			// C0
	UInt32			MultType;		// C4
	UInt32			unkC8;			// C8
	TESObjectREFR*	unkCC;			// CC
	UInt32			unkD0[5];		// D0
};
static_assert(sizeof(BSFadeNode) == 0xE4);

class BSTreeModel : public NiRefObject {
public:
	UInt32					Unk08;				// 08
	UInt32					Unk0C;				// 0C
	UInt32					Unk10;				// 10
	UInt32					Unk14;				// 14
	UInt32					Unk18;				// 18
	UInt32					Unk1C;				// 1C
	UInt32					Unk20;				// 20
	UInt32					Unk24;				// 24
	UInt32					Unk28;				// 28
	void*					Unk2C;				// 2C BSShaderTextureSet*
	NiSourceTexture*		LeavesTexture;		// 30
	void*					Unk34;				// 34 BSShaderTextureSet*
	UInt32					Unk38;				// 38
	float					Unk3C;				// 3C
	UInt32					Unk40;				// 40
	UInt8					Unk44;				// 44
	UInt8					pad44[3];
	float					Unk48;				// 48
	float					Unk4C;				// 4C
};
static_assert(sizeof(BSTreeModel) == 0x50);

class BSTreeNode : public BSFadeNode {
public:
	BSTreeModel*		TreeModel;		// E4
	UInt32				UnkE0;			// E8
	UInt32				UnkE4;			// EC
	UInt32				UnkE8;			// F0
	float				UnkEC;			// F4
};
static_assert(sizeof(BSTreeNode) == 0xF8);

class ShadowSceneLight : public NiRefObject {
public:

	UInt32					unk008;				// 008
	float					flt00C[49];			// 00C
	float					lightFade;
	float					unk0D4;
	float					unk0D8;
	float					unk0DC;
	NiTList<NiTriBasedGeom>	lgtList0E0;			// 0E0
	UInt8					byte0EC;			// 0EC
	UInt8					byte0ED;			// 0ED
	UInt8					byte0EE[2];			// 0EE
	UInt32					unk0F0;				// 0F0
	UInt32					unk0F4;				// 0F4
	NiPointLight*			sourceLight;		// 0F8
	UInt32					unk0FC;				// 0FC
	NiPoint3				fPosition;			// 100
	UInt32					unk10C;				// 10C
	UInt16					bIsEnabled;			// 110
	UInt16					unk112;				// 112
	UInt32					unk114;				// 114
	UInt8					byte118;			// 118
	UInt8					pad119[3];			// 119
	float					flt11C;				// 11C
	float					flt120;				// 120
	UInt8					byte124;			// 124
	UInt8					pad125[3];			// 125
	UInt32					unk128[66];			// 128
	UInt32					array230[4];		// 230 BSSimpleArray<NiNode>
	void*					portalGraph;		// 240 BSPortalGraph*
	UInt32					unk244[3];			// 244
};
static_assert(sizeof(ShadowSceneLight) == 0x250);

class ShadowSceneNode : public NiNode {
public:
	UInt32								unk0AC[2];			// 0AC
	NiTList<ShadowSceneLight>			lights;				// 0B4
	NiTList<ShadowSceneLight>			actorShadowCasters;	// 0C0
	NiTList<ShadowSceneLight>::Entry*	unk0CC;				// 0CC
	NiTList<ShadowSceneLight>::Entry*	unk0D0;				// 0D0
	NiTList<ShadowSceneLight>::Entry*	unk0D4;				// 0D4
	ShadowSceneLight*					sceneLight1;		// 0D8
	ShadowSceneLight*					sceneLight2;		// 0DC
	ShadowSceneLight*					directionalLight;	// 0E0
	UInt32								unk0E4[6];			// 0E4
	void*								ptr0FC;				// 0FC
	void*								ptr100;				// 100
	UInt32								unk104;				// 104
	UInt32								unk108[3];			// 108
	void*								ptr114;				// 114
	void*								ptr118;				// 118
	UInt32								unk11C;				// 11C
	UInt32								unk120;				// 120
	UInt32								unk124;				// 124
	NiCamera*							cubeMapCam;			// 128 BSCubeMapCamera*
	UInt32								unk12C;				// 12C
	UInt8								byte130;			// 130
	UInt8								byte131;			// 131
	UInt8								pad132[2];			// 132
	void*								fogProperty;		// 134 BSFogProperty*
	UInt32								unk138;				// 138
	UInt32								array13C[4];		// 13C BSSimpleArray<NiFrustumPlanes>
	UInt32								array14C[4];		// 14C BSSimpleArray<void>
	UInt32								unk15C[3];			// 15C
	NiVector4							unk168;				// 168
	NiVector4							unk178;				// 178
	NiVector4							unk188;				// 188
	NiVector4							unk198;				// 198
	NiVector4							unk1A8;				// 1A8
	NiVector4							unk1B8;				// 1B8
	UInt32								lightingPasses;		// 1C8
	float								flt1CC[3];			// 1CC
	UInt32								unk1D8;				// 1D8
	UInt8								byte1DC;			// 1DC
	UInt8								pad1DD[3];			// 1DD
	void*								portalGraph;		// 1E0 BSPortalGraph*
	UInt32								unk1E4[3];			// 1E4
	float								flt1F0[3];			// 1F0
	UInt8								byte1FC;			// 1FC
	UInt8								pad1FD[3];			// 1FD
};
static_assert(sizeof(ShadowSceneNode) == 0x200);

class NiCullingProcess {
public:
	void* vftable;
	bool m_bUseVirtualAppend;
	NiVisibleArray* m_pkVisibleSet;
	NiCamera* m_pkCamera;
	NiFrustum m_kFrustum;
	NiFrustumPlanes m_kPlanes;
};

class BSCompoundFrustum;

class BSCullingProcess : public NiCullingProcess {
public:
	UInt32 kCullMode;
	UInt32 eTypeStack[10];
	UInt32 uiStackIndex;
	BSCompoundFrustum* pCompoundFrustum;
	BSShaderAccumulator* spAccumulator;
	bool bRecurseToGeometry;
};
static_assert(offsetof(BSCullingProcess, kCullMode) == 0x90);

class BSShaderManager {
public:
	enum SceneGraphType : UInt32 {
		BSSM_SSN_WORLD = 0,
		BSSM_SSN_MENU_OBJECT_3D = 1,
		BSSM_SSN_UNK_2 = 2,
		BSSM_SSN_MENU_PLAYER_3D = 3,
		BSSM_SSN_COUNT = 4,
	};

	enum ShaderType : UInt32 {
		BSSM_SHADER_DEFAULT = 0,
		BSSM_SHADER_SHADOWLIGHT = 1,
		BSSM_SHADER_TALL_GRASS = 2,
		BSSM_SHADER_DISTANT_LOD = 3,
		BSSM_SHADER_ST_BRANCH = 4,
		BSSM_SHADER_ST_FROND = 5,
		BSSM_SHADER_ST_LEAF = 6,
		BSSM_SHADER_BLOOD_SPLATTER = 7,
		BSSM_SHADER_DISTANT_TREE = 8,
		BSSM_SHADER_DEBUG = 9,
		BSSM_SHADER_SKY = 10,
		//							  11
		//							  12
		BSSM_SHADER_HAIR = 13,
		BSSM_SHADER_SKIN = 14,
		BSSM_SHADER_PARALLAX = 15,
		BSSM_SHADER_GEOM_DECAL = 16,
		BSSM_SHADER_WATER = 17,
		// 						      18
		// 						      19
		// 						      20
		// 						      21
		// 						      22
		// 						      23
		BSSM_SHADER_PARTICLE = 24,
		BSSM_SHADER_BOLT = 25,
		BSSM_SHADER_BEAM = 26,
		//							  27,
		//							  28,
		BSSM_SHADER_LIGHTING30 = 29,
		BSSM_SHADER_PRECIPITATION = 30,
		// 						      31
		BSSM_SHADER_TILE = 32,
		BSSM_SHADER_NOLIGHTING = 33,
		BSSM_SHADER_VOLUMETRIC_FOG = 34
	};

	static float* const fDepthBias;
	static float* const fLODLandDrop;
	static NiPoint3* const kCameraPos;
	static NiPoint4* const kLoadedRange;

	static BSShader** pspShaders;

	static ShadowSceneNode* GetShadowSceneNode(UInt32 aeType);
	static NiDX9Renderer* GetRenderer();
};

#pragma once

// graphics
#include <DXDevice.h>

// bindables
#include <Bindable/Bindable.h>
#include <Bindable/IndexBuffer.h>

// std includes
#include <memory>
#include <vector>

class DrawableBase
{
	template<typename T>
	friend class Drawable;

public:
	DrawableBase() = default;

	virtual inline DX::XMMATRIX GetTransform() const = 0;

	void Draw( const DXDevice &gfx );

protected:
	// for binding statics before drawing	
	virtual std::vector<std::unique_ptr<Bindable>> &GetStaticBinds() const = 0;

	// for setting up
	void AddIndexBuffer( std::unique_ptr<IndexBuffer> iBuf );
	void AddBind( std::unique_ptr<Bindable> bind );

	std::vector<std::unique_ptr<Bindable>> _binds;
	const IndexBuffer *_pIndexBuf = nullptr;
};

// includes for derrived classes
// 
// buffer stuff
#include <Bindable/TransformConstBuf.h>
#include <Bindable/ConstantBuffers.h>
#include <Bindable/CameraConstBuf.h>
#include <Bindable/VertexBuffer.h>
//
// shader stuff
#include <Bindable/VertexShader.h>
#include <Bindable/PixelShader.h>
//
// format stuff
#include <Bindable/InputLayout.h>
#include <Bindable/Topology.h>
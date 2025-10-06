#pragma once

// graphics
#include <DXDevice.h>

// bindables
#include <Bindable/IndexBuffer.h>
#include <Bindable/Bindable.h>

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
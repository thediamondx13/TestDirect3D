#pragma once

// auto includes Bindable.h and thus DXDevice.h
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

	// for drawing
	inline virtual DX::XMMATRIX GetTransform() const = 0;
	virtual void Update( float dt ) = 0;
	void Draw( DXDevice &gfx );

protected:
	// for binding statics before drawing	
	virtual const std::vector<std::unique_ptr<Bindable>> &GetStaticBinds() const = 0;

	// for setting up
	void AddIndexBuf( std::unique_ptr<IndexBuffer> iBuf );
	void AddBind( std::unique_ptr<Bindable> bind );

	std::vector<std::unique_ptr<Bindable>> _binds;
	const IndexBuffer *_pIndexBuf = nullptr;
};
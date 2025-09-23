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
	DrawableBase( const DrawableBase& ) = delete;
	DrawableBase& operator=( const DrawableBase& ) = delete;

	// for drawing
	inline virtual DX::XMMATRIX GetTransform() const = 0;
	virtual void Update( float dt ) = 0;
	void Draw( DXDevice& gfx );

	virtual ~DrawableBase() = default;

protected:
	// for binding statics before drawing	
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const = 0;

	// for setting up
	void AddIndexBuf( std::unique_ptr<IndexBuffer> iBuf );
	void AddBind( std::unique_ptr<Bindable> bind );

	std::vector<std::unique_ptr<Bindable>> binds;
	const IndexBuffer *pIndexBuf = nullptr;
};
#pragma once

#include "DrawableBase.h"

template<typename T>
class Drawable : public DrawableBase
{
public:
	Drawable() = default;

protected:
	static bool StaticsNotInitialized() { return s_binds.empty(); }

	static void AddBindStatic( std::unique_ptr<Bindable> bind )
	{
		s_binds.push_back( std::move( bind ) );
	}

	void AddIndexBufferStatic( std::unique_ptr<IndexBuffer> iBuf )
	{
		_pIndexBuf = iBuf.get();
		s_binds.push_back( std::move( iBuf ) );
	}

	void SetIndexFromStatic()
	{
		for ( const auto &b : s_binds )
		{
			if ( const auto iBuf = dynamic_cast<IndexBuffer *>(b.get()) )
			{
				_pIndexBuf = iBuf;
				return;
			}
		}
	}

	std::vector<std::unique_ptr<Bindable>> &GetStaticBinds() const
	{
		return s_binds;
	}

	static std::vector<std::unique_ptr<Bindable>> s_binds;
};

template<typename T>
std::vector<std::unique_ptr<Bindable>> Drawable<T>::s_binds;

// includes for child classes
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
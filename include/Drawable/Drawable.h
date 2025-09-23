#pragma once

#include "DrawableBase.h"

template<typename T>
class Drawable : public DrawableBase
{
protected:
	static bool InitializeStatic() { return staticBinds.empty(); }

	static void AddBindStatic( std::unique_ptr<Bindable> bind )
	{
		staticBinds.push_back( std::move(bind) );
	}

	void AddIndexBufferStatic( std::unique_ptr<IndexBuffer> iBuf )
	{
		pIndexBuf = iBuf.get();
		staticBinds.push_back( std::move( iBuf ) );
	}

	void SetIndexFromStatic()
	{
		for ( const auto& b : staticBinds )
			if ( const auto iBuf = dynamic_cast<IndexBuffer*>(b.get()) )
			{
				pIndexBuf = iBuf;
				return;
			}
	}

	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const
	{
		return staticBinds;
	}

	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<typename T>
std::vector<std::unique_ptr<Bindable>> Drawable<T>::staticBinds;

// includes for child classes
// 
// buffer stuff
#include <Bindable/TransformConstBuf.h>
#include <Bindable/ConstantBuffers.h>
#include <Bindable/VertexBuffer.h>
//
// shader stuff
#include <Bindable/VertexShader.h>
#include <Bindable/PixelShader.h>
//
// format stuff
#include <Bindable/InputLayout.h>
#include <Bindable/Topology.h>
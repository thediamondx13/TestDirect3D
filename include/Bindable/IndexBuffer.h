#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer( const DXDevice &gfx, const std::vector<UINT> &indices );
	void Bind( const DXDevice &gfx ) override;
	UINT GetCount() const;

protected:
	ComPtr<ID3D11Buffer> _pIndexBuffer;
	UINT _count;
};
#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	IndexBuffer( DXDevice &gfx, const std::vector<UINT> &indices );
	void Bind( DXDevice &gfx ) override;
	UINT GetCount() const;

protected:
	ComPtr<ID3D11Buffer> _pIndexBuffer;
	UINT _count;
};
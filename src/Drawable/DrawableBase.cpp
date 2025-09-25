#include <Drawable/DrawableBase.h>

void DrawableBase::AddIndexBuf( std::unique_ptr<IndexBuffer> iBuf )
{
	_pIndexBuf = iBuf.get();
	_binds.push_back( std::move( iBuf ) );
}

void DrawableBase::AddBind( std::unique_ptr<Bindable> bind )
{
	_binds.push_back( std::move( bind ) );
}

void DrawableBase::Draw( DXDevice& gfx )
{
	// bind static Bindables
	for ( auto& b : GetStaticBinds() ) b->Bind( gfx );

	// bind other Bindables
	for ( auto& b : _binds ) b->Bind( gfx );

	// issue a draw call
	gfx.DrawIndexed( _pIndexBuf->GetCount() );
}
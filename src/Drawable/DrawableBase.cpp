#include <Drawable/DrawableBase.h>

void DrawableBase::AddIndexBuf( std::unique_ptr<IndexBuffer> iBuf )
{
	pIndexBuf = iBuf.get();
	binds.push_back( std::move( iBuf ) );
}

void DrawableBase::AddBind( std::unique_ptr<Bindable> bind )
{
	binds.push_back( std::move( bind ) );
}

void DrawableBase::Draw( DXDevice& gfx )
{
	// bind static Bindables
	for ( auto& b : GetStaticBinds() ) b->Bind( gfx );

	// bind other Bindables
	for ( auto& b : binds ) b->Bind( gfx );

	// issue a draw call
	gfx.DrawIndexed( pIndexBuf->GetCount() );
}
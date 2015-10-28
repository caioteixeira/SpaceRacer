#include "SpriteComponent.h"
#include "Actor.h"
#include <SDL/SDL.h>
#include "Renderer.h"

IMPL_COMPONENT(SpriteComponent, DrawComponent);

SpriteComponent::SpriteComponent(Actor& owner)
	:DrawComponent(owner)
{

}

void SpriteComponent::Draw(Renderer& render)
{
	//Just in case...
	if (!mTexture)
		return;

	//Texture scale matrix
	Matrix4 textureScale = Matrix4::CreateScale(mTexture->GetWidth(), mTexture->GetHeight(), 1.0);

	Matrix4 quadWorldTransform = textureScale * mOwner.GetWorldTransform();

	//Render
	render.DrawSprite(mTexture, quadWorldTransform);
}


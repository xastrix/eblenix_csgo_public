#include "interface.h"

void sprite_manager::init(IDirect3DDevice9* device, const byte* img, const size_t img_size, int width, int height)
{
	m_width = width;
	m_height = height;
	m_device = device;
	m_image = img;
	m_image_size = img_size;
}

void sprite_manager::begin()
{
	if (!m_device || !m_sprite)
		D3DXCreateSprite(m_device, &m_sprite);

	if (!m_texture)
		D3DXCreateTextureFromFileInMemoryEx(m_device, m_image, m_image_size, m_width, m_height,
			D3DX_DEFAULT, 0, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &m_texture);

	m_sprite->Begin(D3DXSPRITE_DONOTMODIFY_RENDERSTATE);
}

void sprite_manager::end()
{
	if (!m_device || !m_sprite)
		return;

	m_sprite->End();
}

void sprite_manager::on_reset()
{
	if (!m_device || !m_sprite || !m_texture)
		return;

	m_sprite->OnLostDevice();

	m_texture->Release();
	m_texture = nullptr;
}

void sprite_manager::on_reset_end()
{
	if (!m_device || !m_sprite)
		return;

	m_sprite->OnResetDevice();
}

void sprite_manager::draw(int x, int y)
{
	if (!m_device || !m_texture || !m_sprite)
		return;

	D3DXMATRIX matrix{};
	D3DXMatrixTranslation(&matrix, static_cast<float>(x), static_cast<float>(y), 0.0f);

	m_sprite->SetTransform(&matrix);
	m_sprite->Draw(m_texture, 0, 0, 0, D3DCOLOR_RGBA(255, 255, 255, 255));
}

int sprite_manager::get_width()
{
	return m_width;
}

int sprite_manager::get_height()
{
	return m_height;
}
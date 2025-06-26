#include "renderer.h"

#include "globals.h"

bool renderer::init(IDirect3DDevice9* device)
{
	return create_objects(device);
}

bool renderer::create_objects(IDirect3DDevice9* device)
{
	if (!m_device)
		m_device = device;

	D3DVIEWPORT9 vp{};
	if (FAILED(m_device->GetViewport(&vp)))
		return false;

	GLOBAL(screen_width) = vp.Width;
	GLOBAL(screen_height) = vp.Height;

	if (FAILED(D3DXCreateLine(m_device, &m_line)))
		return false;

	if (FAILED(m_device->CreateStateBlock(D3DSBT_ALL, &m_block)))
		return false;

	return true;
}

void renderer::begin_render_states()
{
	if (!m_device)
		return;

	D3DVIEWPORT9 vp{ 0, 0, GLOBAL(screen_width), GLOBAL(screen_height), 0.0f, 1.0f };
	m_device->SetViewport(&vp);

	m_block->Capture();

	m_device->SetVertexShader(nullptr);
	m_device->SetPixelShader(nullptr);

	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	m_device->SetRenderState(D3DRS_LIGHTING, false);
	m_device->SetRenderState(D3DRS_FOGENABLE, false);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
	m_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	m_device->SetRenderState(D3DRS_STENCILENABLE, false);

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
	m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, true);

	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, true);

	m_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	m_device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	m_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
	m_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	
	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

void renderer::end_render_states()
{
	m_block->Apply();
}

void renderer::draw_line(float x0, float y0, float x1, float y1, float thickness, color_t color)
{
	D3DXVECTOR2 lines[2] = {
		D3DXVECTOR2(x0, y0), D3DXVECTOR2(x1, y1)
	};

	m_line->SetWidth(thickness);

	m_line->Begin();
	m_line->Draw(lines, 2, color.get());
	m_line->End();
}

void renderer::draw_filled_rect(float x, float y, float w, float h, color_t color)
{
	vertex_t v[4] = {
		{ x, y + h, 0.0f, 1.0f, color.get() },
		{ x, y, 0.0f, 1.0f, color.get() },
		{ x + w, y + h, 0.0f, 1.0f, color.get() },
		{ x + w, y, 0.0f, 1.0f, color.get() }
	};

	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex_t));
}

void renderer::draw_filled_rect_fade(float x, float y, float w, float h, uint8_t flags, color_t first, color_t second)
{
	vertex_t v[4]{};

	v[0] = { x, y + h, 0.0f, 1.0f, first.get() };
	v[1] = { x, y, 0.0f, 1.0f, first.get() };
	v[2] = { x + w, y + h, 0.0f, 1.0f, second.get() };
	v[3] = { x + w, y, 0.0f, 1.0f, second.get() };

	if (flags & GRADIENT_VERTICAL)
	{
		v[0] = { x, y + h, 0.0f, 1.0f, second.get() };
		v[1] = { x, y, 0.0f, 1.0f, first.get() };
		v[2] = { x + w, y + h, 0.0f, 1.0f, second.get() };
		v[3] = { x + w, y, 0.0f, 1.0f, first.get() };
	}

	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex_t));
}

void renderer::draw_corner_box(float x, float y, float w, float h, float cx, float cy, color_t color)
{
	draw_line(x, y, x + (w / cx), y, 1.0f, color);
	draw_line(x, y, x, y + (h / cy), 1.0f, color);

	draw_line(x + w, y, x + w - (w / cx), y, 1.0f, color);
	draw_line(x + w, y, x + w, y + (h / cy), 1.0f, color);

	draw_line(x, y + h, x + (w / cx), y + h, 1.0f, color);
	draw_line(x, y + h, x, y + h - (h / cy), 1.0f, color);

	draw_line(x + w, y + h, x + w - (w / cx), y + h, 1.0f, color);
	draw_line(x + w, y + h, x + w, y + h - (h / cy), 1.0f, color);
}

void renderer::undo()
{
	if (m_line) {
		m_line->Release();
		m_line = nullptr;
	}

	if (m_block) {
		m_block->Release();
		m_block = nullptr;
	}

	if (m_device) {
		m_device->Release();
		m_device = nullptr;
	}
}
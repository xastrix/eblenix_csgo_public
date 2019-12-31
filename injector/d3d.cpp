#include "interface.h"

static std::vector<font_t> font_list = {
	{ Tahoma12px,      12, "Tahoma",  FW_NORMAL, ANTIALIASED_QUALITY },
	{ VerdanaBold12px, 12, "Verdana", FW_BOLD, PROOF_QUALITY },
};

bool d3d::init(IDirect3DDevice9* device)
{
	return create_objects(device);
}

bool d3d::create_objects(IDirect3DDevice9* device)
{
	if (!m_device)
		m_device = device;

	if (FAILED(D3DXCreateLine(m_device, &m_line)))
		return false;

	for (const auto& font : font_list)
	{
		if (FAILED(D3DXCreateFontA(
			m_device,
			font.m_px, 0,
			font.m_weight, 1, 0,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			font.m_quality,
			FF_DONTCARE,
			font.m_name.c_str(),
			&m_fonts[font.m_index])))
			return false;
	}

	if (FAILED(m_device->CreateStateBlock(D3DSBT_ALL, &m_block)))
		return false;

	return true;
}

void d3d::begin_render_states()
{
	if (!m_device)
		return;

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

void d3d::end_render_states()
{
	m_block->Apply();
}

void d3d::draw_line(int x0, int y0, int x1, int y1, D3DCOLOR color)
{
	D3DXVECTOR2 lines[2] = {
		D3DXVECTOR2(x0, y0), D3DXVECTOR2(x1, y1)
	};

	m_line->Begin();
	m_line->Draw(lines, 2, color);
	m_line->End();
}

void d3d::draw_rect(int x, int y, int w, int h, D3DCOLOR color)
{
	draw_line(x, y, x + w, y, color);
	draw_line(x, y, x, y + h, color);
	draw_line(x + w, y, x + w, y + h, color);
	draw_line(x, y + h, x + w, y + h, color);
}

void d3d::draw_filled_rect(int x, int y, int w, int h, D3DCOLOR color)
{
	vertex_t v[4] = {
		{ float(x), float(y + h), 0.0f, 1.0f, color },
		{ float(x), float(y), 0.0f, 1.0f, color },
		{ float(x + w), float(y + h), 0.0f, 1.0f, color },
		{ float(x + w), float(y) , 0.0f, 1.0f, color }
	};

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex_t));
	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
}

void d3d::draw_string(const std::string& string, int x, int y, ID3DXFont* font, D3DCOLOR color)
{
	RECT r{ x, y, x, y };
	RECT or{ x + 1, y + 1, x + 1, y + 1 };

	font->DrawTextA(NULL, string.c_str(), -1, &or, DT_NOCLIP, D3DCOLOR_RGBA(0, 0, 0, 120));
	font->DrawTextA(NULL, string.c_str(), -1, &r, DT_NOCLIP, color);
}

int d3d::get_text_width(const std::string& string, ID3DXFont* font)
{
	RECT r{};
	font->DrawTextA(0, string.c_str(), -1, &r, DT_CALCRECT, 0xffffffff);

	return (r.right - r.left);
}

int d3d::get_text_height(const std::string& string, ID3DXFont* font)
{
	RECT r{};
	font->DrawTextA(0, string.c_str(), -1, &r, DT_CALCRECT, 0xffffffff);

	return (r.bottom - r.top);
}

ID3DXFont* d3d::get_font(const _fonts index)
{
	return m_fonts[index];
}

void d3d::undo()
{
	if (m_line) {
		m_line->Release();
		m_line = nullptr;
	}

	for (const auto& font : font_list)
	{
		if (m_fonts[font.m_index]) {
			m_fonts[font.m_index]->Release();
			m_fonts[font.m_index] = nullptr;
		}
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
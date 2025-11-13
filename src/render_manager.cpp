#include "render_manager.h"

#include "globals.h"
#include "interfaces.h"

static std::vector<font_t> font_list = {
	{ Tahoma12px,     12, "Tahoma",     FW_MEDIUM, ANTIALIASED_QUALITY },
    { Verdana12px,    12, "Verdana",    FW_SEMIBOLD, ANTIALIASED_QUALITY },
	{ Astriumwep12px, 12, "AstriumWep", FW_NORMAL, CLEARTYPE_QUALITY },
	{ Astriumwep16px, 16, "AstriumWep", FW_NORMAL, CLEARTYPE_QUALITY },
	{ Astriumwep25px, 25, "AstriumWep", FW_NORMAL, CLEARTYPE_QUALITY },
};

bool render_manager::init(IDirect3DDevice9* device)
{
	return create_objects(device);
}

bool render_manager::create_objects(IDirect3DDevice9* device)
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

void render_manager::begin_render_states()
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

void render_manager::end_render_states()
{
	m_block->Apply();
}

void render_manager::draw_line(float x0, float y0, float x1, float y1, float thickness, color_t color)
{
	D3DXVECTOR2 lines[2] = {
		D3DXVECTOR2(x0, y0), D3DXVECTOR2(x1, y1)
	};

	m_line->SetWidth(thickness);

	m_line->Begin();
	m_line->Draw(lines, 2, color.get());
	m_line->End();
}

void render_manager::draw_filled_rect(float x, float y, float w, float h, color_t color)
{
	vertex_t v[4] = {
		{ x, y + h, 0.0f, 1.0f, color.get() },
		{ x, y, 0.0f, 1.0f, color.get() },
		{ x + w, y + h, 0.0f, 1.0f, color.get() },
		{ x + w, y , 0.0f, 1.0f, color.get() }
	};

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex_t));
	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);
}

void render_manager::draw_corner_box(float x, float y, float w, float h, float cx, float cy, color_t color)
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

void render_manager::draw_string(const std::string& string, float x, float y, ID3DXFont* font, uint8_t flags, color_t color)
{
	RECT r{ x, y, x, y };
	RECT o_r{ x + 1, y + 1, x + 1, y + 1 };

	if (flags & TEXT_CENTER_X) {
		const auto half_width = get_text_width(string, font) / 2;

		r = { LONG(x - half_width), LONG(y), LONG(x - half_width), LONG(y) };
		o_r = { LONG(x - half_width + 1), LONG(y + 1), LONG(x - half_width + 1), LONG(y + 1) };
	}

	if (flags & TEXT_OUTLINE) {
		font->DrawTextA(NULL, string.c_str(), -1, &o_r, DT_NOCLIP, color_t(0, 0, 0, color.get_alpha()).get());
	}

	font->DrawTextA(NULL, string.c_str(), -1, &r, DT_NOCLIP, color.get());
}

void render_manager::draw_stringW(const std::wstring& string, float x, float y, ID3DXFont* font, uint8_t flags, color_t color)
{
	RECT r{ x, y, x, y };
	RECT o_r{ x + 1, y + 1, x + 1, y + 1 };

	if (flags & TEXT_CENTER_X) {
		const auto half_width = get_text_widthW(string, font) / 2;

		r = { LONG(x - half_width), LONG(y), LONG(x - half_width), LONG(y) };
		o_r = { LONG(x - half_width + 1), LONG(y + 1), LONG(x - half_width + 1), LONG(y + 1) };
	}

	if (flags & TEXT_OUTLINE) {
		font->DrawTextW(NULL, string.c_str(), -1, &o_r, DT_NOCLIP, color_t(0, 0, 0, color.get_alpha()).get());
	}

	font->DrawTextW(NULL, string.c_str(), -1, &r, DT_NOCLIP, color.get());
}

void render_manager::draw_status(const std::string& string, color_t color)
{
	const auto string_font = get_font(Tahoma12px);
	const auto string_width = get_text_width(string, string_font);

	draw_filled_rect(GLOBAL(screen_width) - string_width - 15, 10, GLOBAL(screen_width), 17, color_t(0, 0, 0, 100));
	draw_filled_rect(GLOBAL(screen_width) - string_width - 12, 13, 2, 11, color);

	draw_string(string, GLOBAL(screen_width) - string_width - 5, 12,
		string_font, TEXT_OUTLINE, color_t(255, 255, 255));
}

float render_manager::get_text_width(const std::string& string, ID3DXFont* font)
{
	RECT r{};
	font->DrawTextA(0, string.c_str(), -1, &r, DT_CALCRECT, 0xffffffff);

	return (r.right - r.left);
}

float render_manager::get_text_widthW(const std::wstring& string, ID3DXFont* font)
{
	RECT r{};
	font->DrawTextW(0, string.c_str(), -1, &r, DT_CALCRECT, 0xffffffff);

	return (r.right - r.left);
}

ID3DXFont* render_manager::get_font(const _fonts index)
{
	return m_fonts[index];
}

void render_manager::undo()
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
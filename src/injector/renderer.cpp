#include "renderer.h"

#include <algorithm>

struct vertice_t {
	float x, y, z, rhw;
	D3DCOLOR c;
};

void c_renderer::init(IDirect3DDevice9* device)
{
	m_device = device;
}

void c_renderer::rect(c_vec2 pos, c_vec2 size, c_color c)
{
	vertice_t verts[5] = {
		{ int(pos.x), int(pos.y), 0.01f, 0.01f, c.get() },
		{ int(pos.x + size.x), int(pos.y), 0.01f, 0.01f, c.get() },
		{ int(pos.x + size.x), int(pos.y + size.y), 0.01f, 0.01f, c.get() },
		{ int(pos.x), int(pos.y + size.y), 0.01f, 0.01f, c.get() },
		{ int(pos.x), int(pos.y), 0.01f, 0.01f, c.get() }
	};

	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &verts, 20);
}

void c_renderer::rect_fill(c_vec2 pos, c_vec2 size, c_color c)
{
	vertice_t verts[4] = {
		{ int(pos.x), int(pos.y), 0.01f, 0.01f, c.get() },
		{ int(pos.x + size.x), int(pos.y), 0.01f, 0.01f, c.get() },
		{ int(pos.x), int(pos.y + size.y), 0.01f, 0.01f, c.get() },
		{ int(pos.x + size.x), int(pos.y + size.y), 0.01f, 0.01f, c.get() }
	};

	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &verts, 20);
}

void c_renderer::rect_cornered(c_vec2 pos, c_vec2 size, float radii, c_color c)
{
	const float pi = 3.14159265f;

	float cx = pos.x + size.x / 2.0f;
	float cy = pos.y + size.y / 2.0f;

	std::vector<vertice_t> vec_verts{};

	auto push_arc = [&](float cx, float cy, float r, float start_angle, float end_angle) {
		const int segments = 8;
		for (int i = 0; i <= segments; i++) {
			float t = (float)i / segments;
			float angle = start_angle + t * (end_angle - start_angle);
			float x = cx + std::cosf(angle) * r;
			float y = cy + std::sinf(angle) * r;
			vec_verts.push_back({ x, y, 0.01f, 0.01f, c.get() });
		}
	};

	float r = std::min(radii, std::min(size.x / 2, size.y / 2));

	push_arc(pos.x + r, pos.y + r, r, pi, 1.5f * pi);

	vec_verts.push_back({ pos.x + size.x - r, pos.y, 0.01f, 0.01f, c.get() });

	push_arc(pos.x + size.x - r, pos.y + r, r, 1.5f * pi, 2.0f * pi);
	push_arc(pos.x + size.x - r, pos.y + size.y - r, r, 0, 0.5f * pi);
	push_arc(pos.x + r, pos.y + size.y - r, r, 0.5f * pi, pi);

	vec_verts.push_back({ pos.x, pos.y + r, 0.01f, 0.01f, c.get() });
	vec_verts.push_back({ pos.x + r, pos.y, 0.01f, 0.01f, c.get() });

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_LINESTRIP, vec_verts.size() - 2, vec_verts.data(), sizeof(vertice_t));

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
}

void c_renderer::rect_fill_cornered(c_vec2 pos, c_vec2 size, float radii, c_color c)
{
	const float pi = 3.14159265f;

	float cx = pos.x + size.x / 2.0f;
	float cy = pos.y + size.y / 2.0f;

	std::vector<vertice_t> vec_verts{};
	vec_verts.push_back({ cx, cy, 0.01f, 0.01f, c.get() });

	auto push_arc = [&](float cx, float cy, float r, float start_angle, float end_angle) {
		const int   segments = 8;
		for (int i = 0; i <= segments; i++) {
			float t = (float)i / segments;
			float angle = start_angle + t * (end_angle - start_angle);
			float x = cx + std::cosf(angle) * r;
			float y = cy + std::sinf(angle) * r;

			vec_verts.push_back({ x, y, 0.01f, 0.01f, c.get() });
		}
	};

	float r = std::min(radii, std::min(size.x / 2, size.y / 2));

	push_arc(pos.x + r, pos.y + r, r, pi, 1.5f * pi);
	vec_verts.push_back({ pos.x + size.x - r, pos.y, 0.01f, 0.01f, c.get() });

	push_arc(pos.x + size.x - r, pos.y + r, r, 1.5f * pi, 2.0f * pi);
	vec_verts.push_back({ pos.x + size.x, pos.y + size.y - r, 0.01f, 0.01f, c.get() });

	push_arc(pos.x + size.x - r, pos.y + size.y - r, r, 0, 0.5f * pi);
	vec_verts.push_back({ pos.x + r, pos.y + size.y, 0.01f, 0.01f, c.get() });

	push_arc(pos.x + r, pos.y + size.y - r, r, 0.5f * pi, pi);
	vec_verts.push_back({ pos.x, pos.y + r, 0.01f, 0.01f, c.get() });

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, vec_verts.size() - 2, vec_verts.data(), sizeof(vertice_t));

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
}

void c_renderer::line(c_vec2 a, c_vec2 b, c_color c)
{
	vertice_t verts[2] = {
		{ (int)a.x, (int)a.y, 0.01f, 0.01f, c.get() },
		{ (int)b.x, (int)b.y, 0.01f, 0.01f, c.get() }
	};

	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &verts, 20);
}

void c_renderer::begin()
{
	m_device->CreateStateBlock(D3DSBT_PIXELSTATE, &m_state_block);

	m_state_block->Capture();

	m_device->GetVertexDeclaration(&m_vert_dec);
	m_device->GetVertexShader(&m_vert_shader);

	m_device->SetVertexShader(nullptr);
	m_device->SetPixelShader(nullptr);

	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	m_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);

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

	m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);

	m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

void c_renderer::end()
{
	m_state_block->Apply();
	m_state_block->Release();

	m_device->SetVertexDeclaration(m_vert_dec);
	m_device->SetVertexShader(m_vert_shader);
}

void c_renderer::undo()
{
	if (m_device) {
		m_device->Release();
		m_device = nullptr;
	}
}
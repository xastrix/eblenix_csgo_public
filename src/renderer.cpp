#include "renderer.h"

#include <algorithm>

struct vertice_t {
	float x, y, z, rhw;
	D3DCOLOR c;
};

bool c_renderer::init(IDirect3DDevice9* device)
{
	return restore(device);
}

bool c_renderer::restore(IDirect3DDevice9* device)
{
	m_device = device;

	D3DVIEWPORT9 vp{};
	if (FAILED(m_device->GetViewport(&vp)))
		return false;

	m_screen_size = vec2(vp.Width, vp.Height);

	return true;
}

void c_renderer::rect(vec2 pos, vec2 size, c_color c)
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

void c_renderer::rect_fill(vec2 pos, vec2 size, c_color c)
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

void c_renderer::rect_cornered(vec2 pos, vec2 size, float radii, c_color c)
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

void c_renderer::rect_fill_cornered(vec2 pos, vec2 size, float radii, c_color c)
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

void c_renderer::gradient_v(vec2 pos, vec2 size, c_color c_a, c_color c_b)
{
	vertice_t verts[4] = {
		{ int(pos.x), int(pos.y), 0.01f, 0.01f, c_a.get() },
		{ int(pos.x + size.x), int(pos.y), 0.01f, 0.01f, c_a.get() },
		{ int(pos.x), int(pos.y + size.y), 0.01f, 0.01f, c_b.get() },
		{ int(pos.x + size.x), int(pos.y + size.y), 0.01f, 0.01f, c_b.get() }
	};

	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &verts, 20);
}

void c_renderer::gradient_h(vec2 pos, vec2 size, c_color c_a, c_color c_b)
{
	vertice_t verts[4] = {
		{ int(pos.x), int(pos.y), 0.01f, 0.01f, c_a.get() },
		{ int(pos.x + size.x), int(pos.y), 0.01f, 0.01f, c_b.get() },
		{ int(pos.x), int(pos.y + size.y), 0.01f, 0.01f, c_a.get() },
		{ int(pos.x + size.x), int(pos.y + size.y), 0.01f, 0.01f, c_b.get() }
	};

	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &verts, 20);
}

void c_renderer::gradient_multi_fill(vec2 pos, vec2 size, c_color c_a, c_color c_b, c_color c_c, c_color c_d)
{
	vertice_t verts[4] = {
		{ int(pos.x), int(pos.y), 0.01f, 0.01f, c_a.get() },
		{ int(pos.x + size.x), int(pos.y), 0.01f, 0.01f, c_b.get() },
		{ int(pos.x), int(pos.y + size.y), 0.01f, 0.01f, c_c.get() },
		{ int(pos.x + size.x), int(pos.y + size.y), 0.01f, 0.01f, c_d.get() }
	};

	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, &verts, 20);
}

void c_renderer::gradient_multi(vec2 pos, vec2 size, c_color c_a, c_color c_b, c_color c_c, c_color c_d)
{
	vertice_t verts[5] = {
		{ int(pos.x), int(pos.y), 0.01f, 0.01f, c_a.get() },
		{ int(pos.x + size.x), int(pos.y), 0.01f, 0.01f, c_b.get() },
		{ int(pos.x + size.x), int(pos.y + size.y), 0.01f, 0.01f, c_c.get() },
		{ int(pos.x), int(pos.y + size.y), 0.01f, 0.01f, c_d.get() },
		{ int(pos.x), int(pos.y), 0.01f, 0.01f, c_a.get() }
	};

	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &verts, 20);
}

void c_renderer::line(vec2 a, vec2 b, c_color c)
{
	vertice_t verts[2] = {
		{ (int)a.x, (int)a.y, 0.01f, 0.01f, c.get() },
		{ (int)b.x, (int)b.y, 0.01f, 0.01f, c.get() }
	};

	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_LINELIST, 1, &verts, 20);
}

void c_renderer::circle(vec2 center, float radius, c_color c)
{
	vertice_t verts[CIRCLE_POINTS + 1]{};

	build_lookup_table();

	for (auto i = 0; i <= CIRCLE_POINTS; i++)
		verts[i] =
	{
		center.x + radius * m_lookup_table[i].x,
		center.y - radius * m_lookup_table[i].y,
		0.0f,
		1.0f,
		c.get()
	};

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_LINESTRIP, CIRCLE_POINTS, &verts, sizeof(vertice_t));

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
}

void c_renderer::circle_fill(vec2 center, float radius, c_color c)
{
	vertice_t verts[CIRCLE_POINTS + 1]{};

	build_lookup_table();

	for (auto i = 0; i <= CIRCLE_POINTS; i++)
		verts[i] =
	{
		center.x + radius * m_lookup_table[i].x,
		center.y - radius * m_lookup_table[i].y,
		0.0f,
		1.0f,
		c.get()
	};

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);

	m_device->SetTexture(0, nullptr);
	m_device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, CIRCLE_POINTS, &verts, sizeof(vertice_t));

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
}

void c_renderer::corner_box(vec2 pos, vec2 size, float cx, float cy, c_color c)
{
	line(pos.x, pos.y, pos.x + (size.x / cx), pos.y, c);
	line(pos.x, pos.y, pos.x, pos.y + (size.y / cy), c);

	line(pos.x + size.x, pos.y, pos.x + size.x - (size.x / cx), pos.y, c);
	line(pos.x + size.x, pos.y, pos.x + size.x, pos.y + (size.y / cy), c);

	line(pos.x, pos.y + size.y, pos.x + (size.x / cx), pos.y + size.y, c);
	line(pos.x, pos.y + size.y, pos.x, pos.y + size.y - (size.y / cy), c);

	line(pos.x + size.x, pos.y + size.y, pos.x + size.x - (size.x / cx), pos.y + size.y, c);
	line(pos.x + size.x, pos.y + size.y, pos.x + size.x, pos.y + size.y - (size.y / cy), c);
}

void c_renderer::build_lookup_table()
{
	if (!m_lookup_table.empty())
		return;

	for (auto i = 0; i <= CIRCLE_POINTS; i++)
		m_lookup_table.emplace_back(
			std::cos(2.f * D3DX_PI * (i / static_cast<float>(CIRCLE_POINTS))),
			std::sin(2.f * D3DX_PI * (i / static_cast<float>(CIRCLE_POINTS)))
		);

	for (auto lat = 0; lat < POINTS_SPHERE_LATITUDE; lat++)
	{
		const auto a1 = D3DX_PI * static_cast<float>(lat + 1) / (POINTS_SPHERE_LATITUDE + 1);
		const auto sin1 = std::sin(a1);
		const auto cos1 = std::cos(a1);

		for (auto l = 0; l <= POINTS_SPHERE_LONGITUDE; l++)
		{
			const auto a2 = 2 * D3DX_PI * static_cast<float>(l) / POINTS_SPHERE_LONGITUDE;
			const auto sin2 = std::sin(a2);
			const auto cos2 = std::cos(a2);

			m_lookup_sphere.emplace_back(sin1 * cos2, cos1, sin1 * sin2);
		}
	}
}

void c_renderer::begin()
{
	m_device->CreateStateBlock(D3DSBT_PIXELSTATE, &m_state_block);

	m_state_block->Capture();

	m_device->GetVertexDeclaration(&m_vert_dec);
	m_device->GetVertexShader(&m_vert_shader);

	m_device->SetVertexShader(nullptr);
	m_device->SetPixelShader(nullptr);

	m_device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	m_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	m_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_device->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	m_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	m_device->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);

	m_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

	m_device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
	m_device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
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
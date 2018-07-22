// Polygon.hpp
//
// Author: Felix Naredi
// 2018-07-22 13:15:22 +0200
//

#ifndef plg_Polygon_hpp
#define plg_Polygon_hpp

#include <cmath>
#include <iostream>
#include <vector>
#include <utility>
#include <stdexcept>

#include <wrum/Prim.hpp>
#include <wrum/Buffer.hpp>
#include <wrum/VertexBuffer.hpp>
#include <wrum/Field.hpp>
#include <wrum/Attrib.hpp>
#include <wrum/Compiler.hpp>
#include <wrum/Shader.hpp>

#include "App.hpp"

namespace plg
{
    template <typename VertexBuf, typename IndexBuf>
    class Polygon
    {
	static auto validate_compile(std::string descr)
	{
	    return [&descr](const auto& sh) {
		auto sts = sh.compile_status();
		auto& out = sts ? std::cout : std::cerr;
		out << descr << ": ";
		if(sts) { out << "compiled successfully\n"; }
		else {
		    out << "compile failed\n" << sh.log();
		    throw std::runtime_error("compile failed");
		}
	    };
	}

	static void validate_link(const wrum::Program& prg)
	{
	    auto sts = prg.link_status();
	    auto& out = sts ? std::cout : std::cerr;
	    out << "Linking: ";
	    if(sts) { out << "linked successfully\n"; }
	    else {
		out << "linking failed\n" << prg.log();
		throw std::runtime_error("linking failed");
	    }
	}

	static decltype(auto) make_program()
	{
	    wrum::Compiler compiler;
	    return compiler.link(
		validate_link,
		compiler.compile(
		    validate_compile("Vertex Shader"),
		    GL_VERTEX_SHADER,
		    std::forward<const char*>(
			read_file(PLG_SHADER_DIR "sh_v.glsl").data())),
		compiler.compile(
		    validate_compile("Fragment Shader"),
		    GL_FRAGMENT_SHADER,
		    std::forward<const char*>(
			read_file(PLG_SHADER_DIR "sh_f.glsl").data())));
	}

	std::size_t sides_;
	VertexBuf vb_;
	IndexBuf ib_;
	wrum::Program prg_;
	int dirty_;

	using Vertex = typename VertexBuf::Vertex;
	using Index = wrum::UInt;

	constexpr auto indicies_count() const noexcept { return sides_ * 3; }
    public:

	void set_sides(const int n) noexcept
	{
	    if(n == sides_) { return; }
	    if(n < 3) {
		set_sides(3);
		return;
	    }
	    sides_ = n;

	    std::vector<Vertex> vs(n + 1);
	    auto it_v = vs.begin();
	    *it_v++ = Vertex { { 0, 0 } };
	    const auto r = (2.0 * acos(-1)) / static_cast<float>(n);
	    for(int i = 0; i < n; ++i, ++it_v) {
		*it_v = Vertex { { cos(i * r), sin(i * r) } };
	    }
	    vb_.encode(vs);

	    const auto c = n * 3;
	    std::vector<Index> is(c);
	    auto it_i = is.begin();
	    for(int i = 0; i < c - 1; ++i, ++it_i) {
		auto m = i % 3;
		if(m == 0) { *it_i = 0; }
		else { *it_i = (i / 3) + m; }
	    }
	    *it_i = 1;
	    ib_.encode(is);
	}

	void incr_sides(int n) noexcept { set_sides(sides_ + n); }

	Polygon(VertexBuf&& vb, IndexBuf&& ib)
	    : vb_(std::move(vb)),
	      ib_(std::move(ib)),
	      prg_(make_program()),
	      sides_(0)
	{
	    prg_.use();
	    vb_.locate_fields(prg_);
	    prg_.unuse();
	    set_sides(3);
	}

	void draw() noexcept
	{
	    prg_.use();
	    vb_.bind();
	    ib_.bind();
	    glDrawElements(
		GL_TRIANGLES,
		indicies_count(),
		wrum::TypeOption<Index>::value,
		0);
	    ib_.unbind();
	    vb_.unbind();
	    prg_.unuse();
	}
    };

    auto make_polygon()
    {
	using namespace wrum;
	auto vb = make_vertex_buffer<BufferMemory::Dynamic>(
	    LabelField<Attrib::Float2>("vPosition"));
	Buffer<BufferMemory::Dynamic, GL_ELEMENT_ARRAY_BUFFER> ib;
	return Polygon<decltype(vb), decltype(ib)>(std::move(vb), std::move(ib));
    }
}

#endif /* plg_Polygon_hpp */

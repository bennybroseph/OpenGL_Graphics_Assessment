#ifndef _MASTERHEADER_H_
#define _MASTERHEADER_H_
#pragma once

#include <glm/ext.hpp>

#include <memory>
#include <vector>
#include <map>

#define PI glm::pi<float>()

//#include <memory>
#ifdef _MEMORY_

using std::unique_ptr;
using std::make_unique;

using std::shared_ptr;
using std::make_shared;

using std::weak_ptr;

#endif

//#include <vector>
#ifdef _VECTOR_

using std::vector;

//#include <memory>
#ifdef _MEMORY_

template <typename T>
using vectorPtrU = unique_ptr<vector<T>>;
template <typename T>
using vectorPtrS = shared_ptr<vector<T>>;
template <typename T>
using vectorPtrW = weak_ptr<vector<T>>;

#endif

#endif

//#include <map>
#ifdef _MAP_

using std::map;

//#include <memory>
#ifdef _MEMORY_

template<typename T, typename U>
using mapPtrU = unique_ptr<map<T, U>>;
template<typename T, typename U>
using mapPtrS = shared_ptr<map<T, U>>;
template<typename T, typename U>
using mapPtrW = weak_ptr<map<T, U>>;

#endif

#endif

//#include <glm/detail/type_vec.hpp>
__if_exists(glm::vec2)
{
	using glm::vec2;
	using glm::vec3;
	using glm::vec4;

	//#include <memory>
#ifdef _MEMORY_

	typedef unique_ptr<vec2> vec2PtrU;
	typedef shared_ptr<vec2> vec2PtrS;
	typedef weak_ptr<vec2> vec2PtrW;

	typedef unique_ptr<vec3> vec3PtrU;
	typedef shared_ptr<vec3> vec3PtrS;
	typedef weak_ptr<vec3> vec3PtrW;

	typedef unique_ptr<vec4> vec4PtrU;
	typedef shared_ptr<vec4> vec4PtrS;
	typedef weak_ptr<vec4> vec4PtrW;

#endif
}

//#include <glm/detail/type_mat4x4.hpp>
__if_exists(glm::mat4)
{
	using glm::mat4;

	//#include <memory>
#ifdef _MEMORY_

	typedef unique_ptr<mat4> mat4PtrU;
	typedef shared_ptr<mat4> mat4PtrS;
	typedef weak_ptr<mat4> mat4PtrW;

#endif
}


#endif // _MASTERHEADER_H_

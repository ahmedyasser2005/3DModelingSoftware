#pragma once

#include <DirectXMath.h>
#include <cstdint>

enum class CurveType
{
	CubicBezier = 0,
	RationalBezier = 1,
	BSpline = 2
};

struct CurveData
{
	CurveType Type = CurveType::CubicBezier;
	bool IsVisible = true;

	DirectX::XMFLOAT3 P0 = { -0.5f, 0.0f, 0.0f };
	DirectX::XMFLOAT3 P1 = { -0.166f, 0.5f, 0.0f };
	DirectX::XMFLOAT3 P2 = { 0.166f, 0.5f, 0.0f };
	DirectX::XMFLOAT3 P3 = { 0.5f, 0.0f, 0.0f };

	float W0 = 1.0f;
	float W1 = 1.0f;
	float W2 = 1.0f;
	float W3 = 1.0f;

	float Color[3] = { 1.0f, 1.0f, 1.0f };
	int Segments = 32;
};

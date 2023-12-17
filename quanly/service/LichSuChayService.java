package com.example.quanly.service;

import java.util.List;

import com.example.quanly.entity.LichSuChay;

public interface LichSuChayService {
	List<LichSuChay> getAll();
	
	LichSuChay save(LichSuChay lichSuChay);
}

package com.example.quanly.service;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.example.quanly.entity.LichSuChay;
import com.example.quanly.repository.LichSuChayRepository;

@Service
public class LichSuChayServiceImpl implements LichSuChayService {
	@Autowired
	public LichSuChayRepository lichSuChayRepository;

	@Override
	public List<LichSuChay> getAll() {
		List<LichSuChay> listLichSuChays = lichSuChayRepository.findAll();
		return listLichSuChays;
	}

	@Override
	public LichSuChay save(LichSuChay lichSuChay) {
		if (lichSuChay.getAddress() == null || lichSuChay.getTime() == null
				|| lichSuChay.getAddress().trim().isEmpty()) {
			return null;
		} else {
			LichSuChay saveLichSuChay = lichSuChayRepository.save(lichSuChay);
			return saveLichSuChay;
		}

	}

}

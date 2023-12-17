package com.example.quanly.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import com.example.quanly.entity.LichSuChay;

@Repository
public interface LichSuChayRepository extends JpaRepository<LichSuChay, Integer> {
}

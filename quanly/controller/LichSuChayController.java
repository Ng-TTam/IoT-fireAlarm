package com.example.quanly.controller;

import com.example.quanly.entity.LichSuChay;
import com.example.quanly.service.LichSuChayService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Collections;
import java.util.Comparator;
import java.util.List;

@RestController
@RequestMapping("/api/lichsuchay")
@CrossOrigin(origins = "http://localhost:5500")
public class LichSuChayController {

    @Autowired
    private LichSuChayService lichSuChayService;

    @GetMapping("/list")
    public ResponseEntity<List<LichSuChay>> getList() {
        List<LichSuChay> list = lichSuChayService.getAll();
        Collections.sort(list, Comparator.comparing(LichSuChay::getTime).reversed());
        return ResponseEntity.ok(list);
    }

    @PostMapping("/add")
    public ResponseEntity<LichSuChay> add(@RequestBody LichSuChay lichSuChay) {
        if (lichSuChay.getAddress() == null || lichSuChay.getTime() == null
                || lichSuChay.getAddress().trim().isEmpty()) {
            return ResponseEntity.badRequest().build();
        } else {
            LichSuChay savedLichSuChay = lichSuChayService.save(lichSuChay);
            return ResponseEntity.status(HttpStatus.CREATED).body(savedLichSuChay);
        }
    }
}

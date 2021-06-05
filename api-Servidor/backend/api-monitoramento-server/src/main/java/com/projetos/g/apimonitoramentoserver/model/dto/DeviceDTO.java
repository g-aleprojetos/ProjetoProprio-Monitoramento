package com.projetos.g.apimonitoramentoserver.model.dto;

import com.fasterxml.jackson.annotation.JsonFormat;
import lombok.Getter;
import lombok.Setter;

import org.jetbrains.annotations.NotNull;

import java.time.LocalDate;
import java.time.LocalTime;


public class DeviceDTO {

    @Getter @Setter
    private Long id;

    @NotNull
    @Getter @Setter
    private String local;

    @NotNull
    @Getter @Setter
    private String serial;

    @NotNull
    @Getter @Setter
    private String software;

    @NotNull
    @Getter @Setter
    private Boolean status;

    @NotNull
    @Getter @Setter
    @JsonFormat(shape = JsonFormat.Shape.STRING, pattern = "dd/MM/yyyy")
    private LocalDate dataCreated;

    @NotNull
    @Getter @Setter
    @JsonFormat(shape = JsonFormat.Shape.STRING, pattern = "HH:mm:ss")
    private LocalTime hourCreated;

    @Getter @Setter
    @JsonFormat(shape = JsonFormat.Shape.STRING, pattern = "dd/MM/yyyy")
    private LocalDate dateFinished;

    @Getter @Setter
    @JsonFormat(shape = JsonFormat.Shape.STRING, pattern = "HH:mm:ss")
    private LocalTime hourFinished;

}

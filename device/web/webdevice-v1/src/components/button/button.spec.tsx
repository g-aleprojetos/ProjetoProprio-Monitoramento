import React from 'react';
import '@testing-library/jest-dom';
import { screen } from '@testing-library/dom';
import { render} from '@testing-library/react'
import Button from './index'


describe('Teste no botão', () => {
    test('ao clicar no botão', () => {
        render(<Button>ok</Button>);

        const but = screen.getByText(/ok/i);

        expect(but).toBeInTheDocument();
    });
});


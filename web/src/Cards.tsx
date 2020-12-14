import { Button, Paper, Table, TableBody, TableCell, TableContainer, TableHead, TableRow } from '@material-ui/core';
import React from 'react';

export interface Card {
  uid: string;
  authorized: boolean;
}

interface CardsProps {
  cards: Card[];
};

const Cards: React.FC<CardsProps> = ({ cards }) => {
  async function remove(card: Card) {
    fetch('http://localhost:8000/remove-card', {
      body: JSON.stringify(card),
      headers: {
        "content-type": "application/json"
      },
      method: 'POST'
    });
  }

  return (
    <TableContainer component={Paper}>
      <Table>
        <TableHead>
          <TableRow>
            <TableCell>Uid</TableCell>
            <TableCell align="right"></TableCell>
          </TableRow>
        </TableHead>
        <TableBody>
          {cards.map(card => (
            <TableRow>
              <TableCell>{card.uid}</TableCell>
              <TableCell align="right">
                <Button 
                  variant="outlined" 
                  color="secondary"
                  onClick={() => remove(card)}>
                  Remove
                </Button>
                </TableCell>
            </TableRow>
          ))}
        </TableBody>
      </Table>
    </TableContainer>
  );
};

export default Cards;

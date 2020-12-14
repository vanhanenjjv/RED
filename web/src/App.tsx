import { Container, CssBaseline, Fab, Grid } from '@material-ui/core';
import { Add } from '@material-ui/icons';
import React from 'react';
import Cards, { Card } from './Cards';


const App = () => {
  const [cards, setCards] = React.useState<Card[]>([]);

  React.useEffect(() => {
    const id = setInterval(() => {
      getCards().then(setCards);
    }, 1000);

    return () => clearInterval(id);
  }, []);

  async function getCards(): Promise<Card[]> {
    const response = await fetch('http://localhost:8000/cards');
    const cards = await response.json();

    return cards;
  }

  async function register() {
    await fetch('http://localhost:8000/add-card', {
      method: 'POST'
    });
  }

  return (
    <React.Fragment>
      <CssBaseline />
      <Container maxWidth="sm">
        <Grid container spacing={4} style={{ marginTop: 30 }}>
          <Cards cards={cards} />      
          <Grid item container justify="flex-end">
            <Fab color="primary" aria-label="add" onClick={() => register()}>
              <Add />
            </Fab>
          </Grid>
        </Grid>
      </Container>
     
    </React.Fragment>
  );
}

export default App;

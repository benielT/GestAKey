const GK = require('global-keypress');
 
// instantiate 
const gk = new GK();
 
// launch keypress daemon process 
gk.start();
 
// emitted events by process 
gk.on('press', data => {
  console.log(data);
 
  // example output 
  /*
  { data: 'a' }
  { data: 'b' }
  { data: '1' }
  { data: '2' }
  { data: '<RShift>' }
  { data: 'A' }
  { data: 'B' }
  { data: '!' }
  { data: '@' }
  { data: '[released <RShift>]' }
  { data: '<Enter>' }
  */
});
 
// process error 
gk.on('error', error => {
  console.error(error);
});
 
// process closed 
gk.on('close', () => {
  console.log('closed');
});
 
// manual stop 

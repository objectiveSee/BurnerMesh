const util = require('util');
const exec = util.promisify(require('child_process').exec);

async function uploadTarget(target) {
  
  let command = 'platformio run -e d1_mini_pro -t upload --upload-port '+target;
  // let command = 'sleep 4; echo Done'
  
  console.log('Target '+target+' starting...');
  
  const { stdout, stderr } = await exec(command);
  
  if ( stderr ) {
    console.log('Target '+target+' failed.');
    // console.log('stdout:', stdout);
    // console.log('stderr:', stderr);
  } else {
    console.log('Target '+target+' finished with no STDERR');
  }
}

async function uploadAll() {
  
  let results = await Promise.all([
    uploadTarget('/dev/cu.wchusbserial14210'),
    uploadTarget('/dev/cu.wchusbserial14220'),
    uploadTarget('/dev/cu.wchusbserial14230')
  ])
  
  return results;

}

uploadAll()
.catch(err => {
  console.log('Caught error '+err);
})
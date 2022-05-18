extern crate portaudio;
extern crate regex;
extern crate hound;

use portaudio as pa;
use regex::Regex;
use portaudio::DeviceIndex;

const SAMPLE_RATE: f64 = 48000.0;
const CHANNELS: usize = 2;
const FRAMES: usize = 1024;
const INTEREAVED: bool = true;

fn main() 
{
	match run() { 
		Ok(_) => {}
		e => { 
			eprintln!("Example failed with following: {:?}", e); 
		}
	}
}

fn run() -> Result<(), pa::Error> 
{
	let src = "sample.wav";
	let pa = pa::PortAudio::new()?;

	let default_host = pa.default_host_api()?;
	println!("default host: {:#?}", pa.host_api_info(default_host));

	let devices = pa.devices()?;

	let mut use_output_device_index: DeviceIndex = DeviceIndex(0);

	// let use_output_device_regex = Regex::new(r"Fireface").unwrap();
	let use_output_device_regex = Regex::new(r"ヘッドフォン").unwrap();

	let mut exists_match_device = false;

	for device_result in devices { 
		
		let device = device_result?;
		let device_index = device.0;
		let device_info = device.1;
		
		let device_name = device_info.name;

		if use_output_device_regex.is_match(device_name) { 
			use_output_device_index = device_index.clone();
			exists_match_device = true;
		}

		println!("{:#?}", &device_index);
		println!("{:#?}", &device_info);
	}

	if !exists_match_device { 
		println!("device not found");
	}

	let output_info = pa.device_info(use_output_device_index)?;
	println!("use output device info : {:#?}", &output_info);

	let latency = output_info.default_low_output_latency;
	let output_params = pa::StreamParameters::<f32>::new(use_output_device_index, CHANNELS as i32, INTEREAVED, latency);
	pa.is_output_format_supported(output_params, SAMPLE_RATE)?;
	let output_settings = pa::OutputStreamSettings::new(output_params, SAMPLE_RATE, FRAMES as u32);

	let mut stream = pa.open_blocking_stream(output_settings)?;

	let mut reader = hound::WavReader::open(src).unwrap();

	let wav_buffer: Vec<i16> = reader.samples().map(|s| s.unwrap()).collect();

	let mut wav_buffer_iter = wav_buffer.iter();

	stream.start()?;

	'stream: loop { 
		let n_write_samples = FRAMES as usize * CHANNELS  as usize; 
		
		stream.write(FRAMES as u32, |output| { 
			for i in 0..n_write_samples { 

				let data = match wav_buffer_iter.next() { 
					Some(t) => *t as f32, 
					None => { 
						panic!("wav file is end.");
					}, 
				};

				output[i] = 0.1 * (data / 32767.0); 
			}
		})?;
	}
}

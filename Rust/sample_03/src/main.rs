extern crate hound;
extern crate portaudio;

use portaudio as pa;
use std::i16;
use std::sync::mpsc;

const SAMPLE_RATE: f64 = 44100.0;
const CHANNELS: i32 = 1;
const INTERLEAVED: bool = true;
const FREAMES_PER_BUFFER: u32 = 64;
const BITS_PER_SAMPLE: u16 = 16;
const NUM_SECONDS: i32 = 10;
const BUF_SIZE: usize = SAMPLE_RATE as usize * NUM_SECONDS as usize;

fn main() {
	match run() {
		Ok(_) => {},
		e => {
			eprintln!("Example failed with the following: {:?}", e);
		}
	}
}

fn run() -> Result<(), pa::Error> {
	let pa = pa::PortAudio::new()?;

	println!("PortAudio:");
	println!("version: {}", pa.version());
	println!("version text: {:?}", pa.version_text());
	println!("host count: {}", pa.host_api_count()?);

	let default_host = pa.default_host_api()?;
	println!("default host: {:#?}", pa.host_api_info(default_host));

	let def_input = pa.default_input_device()?;
	let input_info = pa.device_info(def_input)?;
	println!("Default input device info: {:#?}", &input_info);

	let latency = input_info.default_low_input_latency;
	let input_params = pa::StreamParameters::<f32>::new(def_input, CHANNELS, INTERLEAVED, latency);


	pa.is_input_format_supported(input_params, SAMPLE_RATE)?;

	let settings = pa.default_input_stream_settings(CHANNELS, SAMPLE_RATE, FREAMES_PER_BUFFER)?;
	let mut counter = 0;
	let (tx, rx) = mpsc::channel();
	let callback = move | pa::InputStreamCallbackArgs { buffer, frames, .. } | { 
		let mut idx = 0;
		let mut data: Vec<f32> = vec![];
		for _ in 0..frames {
			if counter >= BUF_SIZE {
				break;
			}
			data.push(buffer[idx]);
			data.push(buffer[idx+1]);
			counter += 1;
			idx += 2;
		}
		tx.send(data).ok();
		if counter >= BUF_SIZE {
			println!("complete rec.");
			pa::Complete
		} else {
			pa::Continue
		}
	};

	let mut stream = pa.open_non_blocking_stream(settings, callback)?;

	let spec = hound::WavSpec {
		channels: CHANNELS as u16, 
		sample_rate: SAMPLE_RATE as u32, 
		bits_per_sample: BITS_PER_SAMPLE, 
		sample_format: hound::SampleFormat::Int, 
	};

	let mut writer = hound::WavWriter::create("./rec.wav", spec).unwrap();

	stream.start()?;
	println!("play {} seconds..", NUM_SECONDS);

	while let true = stream.is_active()? {
		while let Ok(data) = rx.try_recv() {
			for d in &data {
				let amplitude = i16::MAX as f32;
				writer.write_sample((d * amplitude) as i16).unwrap();
			}
		}
	}

	stream.stop()?;
	stream.close()?;
	writer.finalize().unwrap();
	println!("finished.");
	Ok(())
}

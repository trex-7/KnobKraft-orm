/*
   Copyright (c) 2020 Christof Ruch. All rights reserved.

   Dual licensed: Distributed under Affero GPL license by default, an MIT license is available for purchase
*/

#include "GenericAdaption.h"

#include "Patch.h"
#include "Logger.h"

#include <pybind11/stl.h>
//#include <pybind11/pybind11.h>
#include <memory>

namespace py = pybind11;

#include <boost/format.hpp>

namespace knobkraft {

	std::unique_ptr<py::scoped_interpreter> sGenericAdaptionPythonEmbeddedGuard;

	class GenericPatchNumber : public midikraft::PatchNumber {
	public:
		GenericPatchNumber(MidiProgramNumber programNumber) : programNumber_(programNumber) {
		}

		std::string friendlyName() const override
		{
			return (boost::format("%d") % programNumber_.toOneBased()).str();
		}

	private:
		MidiProgramNumber programNumber_;
	};

	class GenericPatch : public midikraft::Patch {
	public:
		GenericPatch(std::string const &name, midikraft::Synth::PatchData const &data) : midikraft::Patch(0, data), name_(name) {
			patchNumber_ = std::make_shared<GenericPatchNumber>(MidiProgramNumber::fromZeroBase(0));
		}

		std::string name() const override
		{
			return name_;
		}

		std::shared_ptr<midikraft::PatchNumber> patchNumber() const override
		{
			return patchNumber_;
		}


		void setPatchNumber(MidiProgramNumber patchNumber) override
		{
			patchNumber_ = std::make_shared<GenericPatchNumber>(patchNumber);
		}


		std::vector<std::shared_ptr<midikraft::SynthParameterDefinition>> allParameterDefinitions() override
		{
			return {};
		}

	private:
		std::string name_;
		std::shared_ptr<midikraft::PatchNumber> patchNumber_;
	};

	GenericAdaption::GenericAdaption(std::string const &pythonModuleFilePath) : filepath_(pythonModuleFilePath)
	{
		try {
			adaption_module = py::module::import(filepath_.c_str());
		}
		catch (py::error_already_set &ex) {
			SimpleLogger::instance()->postMessage((boost::format("Failure loading python module: %s") % ex.what()).str());
		}
	}

	void GenericAdaption::startupGenericAdaption()
	{
		sGenericAdaptionPythonEmbeddedGuard = std::make_unique<py::scoped_interpreter>();
		py::exec("import sys\nsys.path.append(\"d:/Development/github/KnobKraft-Orm/adaptions\")\n");
	}


	int GenericAdaption::numberOfBanks() const
	{
		try {
			py::object result = adaption_module.attr("numberOfBanks")();
			return result.cast<int>();
		}
		catch (std::exception &ex) {
			SimpleLogger::instance()->postMessage((boost::format("Error calling numberOfBanks: %s") % ex.what()).str());
			return 1;
		}
	}

	int GenericAdaption::numberOfPatches() const
	{
		try {
			py::object result = adaption_module.attr("numberOfPatchesPerBank")();
			return result.cast<int>();
		}
		catch (std::exception &ex) {
			SimpleLogger::instance()->postMessage((boost::format("Error calling numberOfPatchesPerBank: %s") % ex.what()).str());
			return 0;
		}
	}

	std::string GenericAdaption::friendlyBankName(MidiBankNumber bankNo) const
	{
		//TODO could delegate this to the python code as well
		return (boost::format("Bank %d") % bankNo.toOneBased()).str();
	}

	std::shared_ptr<midikraft::DataFile> GenericAdaption::patchFromPatchData(const Synth::PatchData &data, MidiProgramNumber place) const
	{
		auto patch = std::make_shared<GenericPatch>(getName() + " Patch", data);
		patch->setPatchNumber(place);
		return patch;
	}

	bool GenericAdaption::isOwnSysex(MidiMessage const &message) const
	{
		//TODO - if we delegate this to the python code, the "sniff synth" method of the Librarian can be used. But this is currently disabled anyway,
		// even if I forgot why
		ignoreUnused(message);
		return false;
	}

	juce::MidiMessage GenericAdaption::deviceDetect(int channel)
	{
		try {
			py::object result = adaption_module.attr("createDeviceDetectMessage")(channel);
			return vectorToMessage(result.cast<std::vector<int>>());
		}
		catch (std::exception &ex) {
			SimpleLogger::instance()->postMessage((boost::format("Error calling createDeviceDetectMessage: %s") % ex.what()).str());
			return MidiMessage();
		}
	}

	int GenericAdaption::deviceDetectSleepMS()
	{
		try
		{
			py::object result = adaption_module.attr("deviceDetectWaitMilliseconds")();
			return result.cast<int>();

		}
		catch (std::exception &ex) {
			SimpleLogger::instance()->postMessage((boost::format("Error calling deviceDetectSleepMS: %s") % ex.what()).str());
			return 100;
		}
	}

	MidiChannel GenericAdaption::channelIfValidDeviceResponse(const MidiMessage &message)
	{
		try {
			py::object result = adaption_module.attr("channelIfValidDeviceResponse")(messageToVector(message));
			int intResult = result.cast<int>();
			if (intResult >= 0 && intResult < 16) {
				return MidiChannel::fromZeroBase(intResult);
			}
			else {
				return MidiChannel::invalidChannel();
			}
		}
		catch (std::exception &ex) {
			SimpleLogger::instance()->postMessage((boost::format("Error calling channelIfValidDeviceResponse: %s") % ex.what()).str());
			return MidiChannel::invalidChannel();
		}
	}

	bool GenericAdaption::needsChannelSpecificDetection()
	{
		try
		{
			py::object result = adaption_module.attr("needsChannelSpecificDetection")();
			return result.cast<bool>();
		}
		catch (std::exception &ex) {
			SimpleLogger::instance()->postMessage((boost::format("Error calling needsChannelSpecificDetection: %s") % ex.what()).str());
			return true;
		}
	}

	std::shared_ptr<midikraft::Patch> GenericAdaption::patchFromProgramDumpSysex(const MidiMessage& message) const
	{
		// For the Generic Adaption, this is a nop, as we do not unpack the MidiMessage, but rather store the raw MidiMessage
		midikraft::Synth::PatchData data(message.getRawData(), message.getRawData() + message.getRawDataSize());
		return std::make_shared<GenericPatch>(getName() + " Patch", data);
	}

	std::vector<juce::MidiMessage> GenericAdaption::patchToProgramDumpSysex(const midikraft::Patch &patch) const
	{
		// For the Generic Adaption, this is a nop, as we do not unpack the MidiMessage, but rather store the raw MidiMessage(s)
		return { MidiMessage(patch.data().data(), (int) patch.data().size()) };
	}

	std::string GenericAdaption::getName() const
	{
		try {
			py::object result = adaption_module.attr("name")();
			return result.cast<std::string>();
		}
		catch (std::exception &ex) {
			SimpleLogger::instance()->postMessage((boost::format("Error calling getName: %s") % ex.what()).str());
			return "Generic";
		}
	}

	std::vector<juce::MidiMessage> GenericAdaption::requestPatch(int patchNo)
	{
		try {
			py::object result = adaption_module.attr("createProgramDumpRequest")(channel().toZeroBasedInt(), patchNo);
			// These should be only one midi message...
			return { vectorToMessage(result.cast<std::vector<int>>()) };
		}
		catch (std::exception &ex) {
			SimpleLogger::instance()->postMessage((boost::format("Error calling createProgramDumpRequest: %s") % ex.what()).str());
			return {};
		}
	}

	bool GenericAdaption::isSingleProgramDump(const MidiMessage& message) const
	{
		try {
			py::object result = adaption_module.attr("isSingleProgramDump")(messageToVector(message));
			return result.cast<bool>();
		}
		catch (std::exception &ex) {
			SimpleLogger::instance()->postMessage((boost::format("Error calling isSingleProgramDump: %s") % ex.what()).str());
			return false;
		}
	}

	std::vector<int> GenericAdaption::messageToVector(MidiMessage const &message) {
		return std::vector<int>(message.getRawData(), message.getRawData() + message.getRawDataSize());
	}

	std::vector<uint8> GenericAdaption::intVectorToByteVector(std::vector<int> const &data) {
		std::vector<uint8> byteData;
		for (int byte : data) {
			if (byte >= 0 && byte < 256) {
				byteData.push_back((uint8)byte);
			}
			else {
				throw new std::runtime_error("Value out of range in Midi Message");
			}
		}
		return byteData;
	}

	juce::MidiMessage GenericAdaption::vectorToMessage(std::vector<int> const &data)
	{
		auto byteData = intVectorToByteVector(data);
		return MidiMessage(byteData.data(), (int)byteData.size());
	}


}
